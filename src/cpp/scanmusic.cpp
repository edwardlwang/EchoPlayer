//
// Created by edward on 2026/4/16.
//

#include "../h/scanmusic.h"

#include <QDateTime>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QMutexLocker>
#include <QSet>
#include <QtConcurrent/QtConcurrentRun>

// TagLib：读取标题、歌手、专辑、时长（行业常用方案）
#include <taglib/audioproperties.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace {
    // 支持的音频扩展名（小写）
    const QSet<QString> kAudioExtensions = {
        "mp3", "flac", "wav", "m4a", "ogg", "aac", "wma", "opus"
    };

    // 统一把 TagLib 字符串转成 QString
    QString fromTagLibString(const TagLib::String &s) {
        return QString::fromUtf8(s.toCString(true)).trimmed();
    }

    bool isAudioFileBySuffix(const QString &fileName) {
        const QString ext = QFileInfo(fileName).suffix().toLower();
        return kAudioExtensions.contains(ext);
    }

    // 单文件读取：优先用 TagLib 标签，失败时回退到文件名。
    SongInfo readSongWithTagLib(const QString &filePath, bool &metadataOk) {
        SongInfo s;
        const QFileInfo fi(filePath);

        s.file_path = filePath;
        s.title = fi.completeBaseName();
        s.artist = QStringLiteral("未知歌手");
        s.album = QStringLiteral("-");
        s.duration_sec = 0;

        metadataOk = false;

        const QByteArray localPath = QDir::toNativeSeparators(filePath).toLocal8Bit();
        TagLib::FileRef ref(localPath.constData());
        if (ref.isNull()) {
            return s;
        }

        if (TagLib::Tag *tag = ref.tag()) {
            const QString title = fromTagLibString(tag->title());
            const QString artist = fromTagLibString(tag->artist());
            const QString album = fromTagLibString(tag->album());

            if (!title.isEmpty()) {
                s.title = title;
            }
            if (!artist.isEmpty()) {
                s.artist = artist;
            }
            if (!album.isEmpty()) {
                s.album = album;
            }
        }

        if (TagLib::AudioProperties *props = ref.audioProperties()) {
            s.duration_sec = props->lengthInSeconds();
        }

        metadataOk = true;
        return s;
    }

    QDir::Filters makeFileFilters(bool includeHidden) {
        QDir::Filters filters = QDir::Files | QDir::Readable | QDir::NoDotAndDotDot;
        if (includeHidden) {
            filters |= QDir::Hidden;
        }
        return filters;
    }
}

QString ScanMusic::resolveMusicPath(QString path) {
    path = path.trimmed();
    if (path.startsWith("~/")) {
        path.replace(0, 1, QDir::homePath());
    } else if (path == "~") {
        path = QDir::homePath();
    }

    if (path.isEmpty()) {
        path = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    }
    return path;
}

QVector<SongInfo> ScanMusic::scanMusic(const QString &path) {
    ScanOptions options;
    options.rootPath = path;
    return scanMusicSync(options, nullptr, nullptr);
}

QVector<SongInfo> ScanMusic::scanMusicSync(
    const ScanOptions &options,
    ScanStats *statsOut,
    std::atomic_bool *cancelFlag
) {
    QElapsedTimer timer;
    timer.start();

    ScanStats stats;
    QVector<SongInfo> songs;

    const QString root = resolveMusicPath(options.rootPath);
    if (root.isEmpty() || !QDir(root).exists()) {
        qWarning() << "Music folder not found:" << root;
        if (statsOut) {
            stats.elapsedMs = timer.elapsed();
            *statsOut = stats;
        }
        return songs;
    }

    const QDirIterator::IteratorFlags iteratorFlags =
        options.recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;

    QDirIterator it(root, makeFileFilters(options.includeHidden), iteratorFlags);
    while (it.hasNext()) {
        if (cancelFlag && cancelFlag->load()) {
            break;
        }

        const QString filePath = it.next();
        ++stats.filesVisited;

        if (!isAudioFileBySuffix(filePath)) {
            continue;
        }

        ++stats.audioMatched;

        bool metadataOk = false;
        SongInfo s = readSongWithTagLib(filePath, metadataOk);
        if (metadataOk) {
            ++stats.metadataOk;
        } else {
            ++stats.metadataFallback;
        }

        songs.push_back(std::move(s));
    }

    stats.elapsedMs = timer.elapsed();
    if (statsOut) {
        *statsOut = stats;
    }

    qDebug() << "Scanned songs:" << songs.size()
             << "from" << root
             << "visited" << stats.filesVisited
             << "elapsed(ms)" << stats.elapsedMs;

    return songs;
}

ScanMusic::ScannerService::ScannerService(QObject *parent)
    : QObject(parent) {
    // queued signal 传自定义类型前，注册一次即可。
    qRegisterMetaType<SongInfo>("SongInfo");
    qRegisterMetaType<QVector<SongInfo>>("QVector<SongInfo>");
    qRegisterMetaType<ScanStats>("ScanMusic::ScanStats");
}

ScanMusic::ScannerService::~ScannerService() {
    cancel();
    m_watcher.waitForFinished();
}

void ScanMusic::ScannerService::startScan(const ScanOptions &options) {
    if (m_watcher.isRunning()) {
        cancel();
        m_watcher.waitForFinished();
    }

    m_cancelRequested.store(false);
    m_watcher.setFuture(QtConcurrent::run([this, options]() {
        runWorker(options);
    }));
}

void ScanMusic::ScannerService::cancel() {
    m_cancelRequested.store(true);
}

bool ScanMusic::ScannerService::isRunning() const {
    return m_watcher.isRunning();
}

void ScanMusic::ScannerService::runWorker(ScanOptions options) {
    QElapsedTimer timer;
    timer.start();

    ScanStats stats;
    QVector<SongInfo> allSongs;
    QVector<SongInfo> batch;

    if (options.batchSize <= 0) {
        options.batchSize = 100;
    }

    const QString root = resolveMusicPath(options.rootPath);
    if (root.isEmpty() || !QDir(root).exists()) {
        stats.elapsedMs = timer.elapsed();
        emit finished(allSongs, stats, false);
        return;
    }

    const QDirIterator::IteratorFlags iteratorFlags =
        options.recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;

    QDirIterator it(root, makeFileFilters(options.includeHidden), iteratorFlags);
    while (it.hasNext()) {
        if (m_cancelRequested.load()) {
            break;
        }

        const QString filePath = it.next();
        ++stats.filesVisited;
        emit progress(stats.filesVisited, 0);

        if (!isAudioFileBySuffix(filePath)) {
            continue;
        }

        ++stats.audioMatched;

        SongInfo song;
        bool metadataOk = false;

        const QFileInfo fi(filePath);
        if (options.useCache) {
            const FileSignature sig = makeSignature(fi);
            bool cacheHit = false;

            {
                QMutexLocker locker(&m_cacheMutex);
                const auto itCache = m_cache.constFind(filePath);
                if (itCache != m_cache.constEnd() && itCache->sig == sig) {
                    song = itCache->song;
                    cacheHit = true;
                }
            }

            if (!cacheHit) {
                song = readOneFile(filePath, metadataOk);
                QMutexLocker locker(&m_cacheMutex);
                m_cache.insert(filePath, CachedEntry{sig, song});
            } else {
                // 命中缓存时，认为之前已完成元数据解析。
                metadataOk = true;
            }
        } else {
            song = readOneFile(filePath, metadataOk);
        }

        if (metadataOk) {
            ++stats.metadataOk;
        } else {
            ++stats.metadataFallback;
        }

        allSongs.push_back(song);
        batch.push_back(song);

        if (batch.size() >= options.batchSize) {
            emit batchReady(batch);
            batch.clear();
        }
    }

    if (!batch.isEmpty()) {
        emit batchReady(batch);
    }

    stats.elapsedMs = timer.elapsed();
    emit finished(allSongs, stats, m_cancelRequested.load());
}

SongInfo ScanMusic::ScannerService::readOneFile(const QString &filePath, bool &metadataOk) const {
    return readSongWithTagLib(filePath, metadataOk);
}

ScanMusic::ScannerService::FileSignature
ScanMusic::ScannerService::makeSignature(const QFileInfo &fi) {
    return FileSignature{fi.size(), fi.lastModified().toUTC()};
}

