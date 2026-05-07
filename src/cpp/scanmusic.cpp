//
// Created by edward on 2026/4/16.
//

#include "../h/scanmusic.h"

#include <QDateTime>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>
#include <QSet>
#include <QtConcurrent/QtConcurrentRun>

// TagLib：读取标题、歌手、专辑、时长（行业常用方案）
#include <taglib/audioproperties.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>        // MPEG::File
#include <taglib/id3v2tag.h>        // ID3v2::Tag
#include <taglib/attachedpictureframe.h> // AttachedPictureFrame

#include <taglib/flacfile.h>        // FLAC::File
#include <taglib/flacpicture.h>     // FLAC::Picture

namespace {
    // 支持的音频扩展名（小写）
    const QSet<QString> kAudioExtensions = {
        "mp3", "flac", "wav", "m4a", "ogg", "aac", "wma", "opus"
    };

    // 统一把 TagLib 字符串转成 QString
    QString fromTagLibToString(const TagLib::String &s) {
        return QString::fromUtf8(s.toCString(true)).trimmed();
    }

    // 判断文件是否为音频文件
    bool isAudioFileBySuffix(const QString &fileName) {
        const QString ext = QFileInfo(fileName).suffix().toLower();
        return kAudioExtensions.contains(ext);
    }

    // 单文件读取：优先用 TagLib 标签，失败时回退到文件名。
    SongInfo readSongWithTagLib(const QString &file_path, bool &meta_data_ok) {
        SongInfo song_info;

        song_info.cover = QImage(":/imgs/album.png");
        song_info.file_path = file_path;
        song_info.title = QFileInfo(file_path).completeBaseName();
        song_info.artist = QStringLiteral("未知歌手");
        song_info.album = QStringLiteral("-");
        song_info.duration_sec = 0;

        meta_data_ok = false;

#ifdef Q_OS_WIN
        // Use UTF-16 path on Windows to avoid lossy local-8bit conversion.
        const std::wstring nativePath = QDir::toNativeSeparators(file_path).toStdWString();
        TagLib::FileRef ref(nativePath.c_str());
#else
        const QByteArray nativePath = QFile::encodeName(QDir::toNativeSeparators(file_path));
        TagLib::FileRef ref(nativePath.constData());
#endif
        if (ref.isNull()) {
            return song_info;
        }

        if (TagLib::Tag *tag = ref.tag()) {
            // const QPixmap cover =


            const QString title = fromTagLibToString(tag->title());
            const QString artist = fromTagLibToString(tag->artist());
            const QString album = fromTagLibToString(tag->album());

            if (!title.isEmpty()) {
                song_info.title = title;
            }
            if (!artist.isEmpty()) {
                song_info.artist = artist;
            }
            if (!album.isEmpty()) {
                song_info.album = album;
            }
        }

        // ---- 仅提取 MP3 和 FLAC 的嵌入式封面 ----
        TagLib::File *file = ref.file();
        QByteArray coverBytes;
        if (file) {
            // --- MP3: ID3v2 标签中的 APIC (Attached Picture) 帧 ---
            if (auto *mpegFile = dynamic_cast<TagLib::MPEG::File *>(file)) {
                if (mpegFile->ID3v2Tag()) {
                    const auto &frameListMap = mpegFile->ID3v2Tag()->frameListMap();
                    auto it = frameListMap.find("APIC");
                    if (it != frameListMap.end() && !it->second.isEmpty()) {
                        auto *pic = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(
                            it->second.front());
                        if (pic) {
                            const TagLib::ByteVector &data = pic->picture();
                            coverBytes = QByteArray(data.data(), static_cast<int>(data.size()));
                        }
                    }
                }
            }
            // --- FLAC: 内置 Picture 列表（通常取第一张） ---
            else if (auto *flacFile = dynamic_cast<TagLib::FLAC::File *>(file)) {
                const TagLib::List<TagLib::FLAC::Picture *> &pics = flacFile->pictureList();
                if (!pics.isEmpty()) {
                    const TagLib::FLAC::Picture *pic = pics.front();
                    const TagLib::ByteVector &data = pic->data();
                    coverBytes = QByteArray(data.data(), static_cast<int>(data.size()));
                }
            }
        }

        if (!coverBytes.isEmpty()) {
            QImage cover;
            if (cover.loadFromData(coverBytes)) {
                song_info.cover = cover;
            } else {
                qWarning() << "Cannot decode cover image for:" << file_path;
            }
        }
        // coverBytes 为空时，song_info.cover 保持默认的 album.png

        if (TagLib::AudioProperties *props = ref.audioProperties()) {
            song_info.duration_sec = props->lengthInSeconds();
        }

        meta_data_ok = true;
        return song_info;
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
    qRegisterMetaType<QVector<SongInfo> >("QVector<SongInfo>");
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

SongInfo ScanMusic::ScannerService::readOneFile(const QString &filePath, bool &metadataOk) {
    return readSongWithTagLib(filePath, metadataOk);
}

ScanMusic::ScannerService::FileSignature
ScanMusic::ScannerService::makeSignature(const QFileInfo &fi) {
    return FileSignature{fi.size(), fi.lastModified().toUTC()};
}

