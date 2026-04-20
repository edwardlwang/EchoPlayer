//
// Created by edward on 2026/4/12.
//

#include "../h/songpage.h"
#include "../h/scanmusic.h"

#include <QFileInfo>
#include <QHeaderView>

SongPage::SongPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "SongPage构造完成";
    this->setObjectName("songpage");
    this->setAttribute(Qt::WA_StyledBackground, true);

    m_main_layout = new QVBoxLayout(this);
    m_main_layout->setContentsMargins(0, 0, 0, 0);
    m_main_layout->setSpacing(0);
    this->setLayout(m_main_layout);

    m_song_table = new QTableWidget(this);
    m_song_table->setColumnCount(4);
    m_song_table->setHorizontalHeaderLabels({"歌曲", "歌手", "专辑", "时长"});
    m_song_table->horizontalHeader()->setStretchLastSection(true);
    m_song_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_song_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_main_layout->addWidget(m_song_table);

    // 同步版本（会阻塞 UI），先保留作对照：
    // setSongs(ScanMusic::scanMusic(QString()));

    // 异步版本：后台扫描，UI 通过信号逐步收到数据。
    m_scanner = new ScanMusic::ScannerService(this);

    connect(m_scanner, &ScanMusic::ScannerService::batchReady,
            this, [this](const QVector<SongInfo> &batch) {
                appendSongsBatch(batch);
            });

    connect(m_scanner, &ScanMusic::ScannerService::finished,
            this, [this](const QVector<SongInfo> &allSongs,
                         const ScanMusic::ScanStats &stats,
                         const bool canceled) {
                if (canceled) {
                    qDebug() << "Song scan canceled, partial songs:" << m_songs.size();
                    return;
                }

                // finished 会给到全量结果；批量阶段如果有漏项，这里做最终对齐。
                if (allSongs.size() != m_songs.size()) {
                    setSongs(allSongs);
                }

                qDebug() << "Song scan finished. songs:" << allSongs.size()
                         << "visited:" << stats.filesVisited
                         << "matched:" << stats.audioMatched
                         << "metadata ok:" << stats.metadataOk
                         << "fallback:" << stats.metadataFallback
                         << "elapsed(ms):" << stats.elapsedMs;
            });

    ScanMusic::ScanOptions options;
    options.rootPath = QString();
    options.recursive = true;
    options.batchSize = 120;
    m_scanner->startScan(options);
}

SongPage::~SongPage() {
    if (m_scanner && m_scanner->isRunning()) {
        m_scanner->cancel();
    }
    qDebug() << "SongPage析构完成";
}

void SongPage::addSong(const QString &title, const QString &artist, const QString &album,
                       const int &duration_sec) const {
    const int row = m_song_table->rowCount();
    m_song_table->insertRow(row);

    const int min = duration_sec / 60;
    const int sec = duration_sec % 60;

    const QString duration = QString("%1:%2")
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0'));

    // addSong 只负责“渲染一行”，不做扫描/刷新，避免递归调用。
    m_song_table->setItem(row, 0, new QTableWidgetItem(title));
    m_song_table->setItem(row, 1, new QTableWidgetItem(artist));
    m_song_table->setItem(row, 2, new QTableWidgetItem(album));
    m_song_table->setItem(row, 3, new QTableWidgetItem(duration));
}

void SongPage::setSongs(const QVector<SongInfo> &songs) {
    m_songs = songs;
    m_song_table->setRowCount(0);

    // setSongs 负责批量刷新表格。
    for (const SongInfo &song: m_songs) {
        QString title = song.title;

        if (title.trimmed().isEmpty()) {
            title = QFileInfo(song.file_path).completeBaseName();
        }

        const QString artist = song.artist.trimmed().isEmpty() ? "未知歌手" : song.artist;
        const QString album = song.album.trimmed().isEmpty() ? "-" : song.album;

        addSong(title, artist, album, song.duration_sec);
    }

    qDebug() << "SongPage loaded songs:" << m_songs.size();
}

void SongPage::appendSongsBatch(const QVector<SongInfo> &songs) {
    for (const SongInfo &song: songs) {
        m_songs.push_back(song);

        QString title = song.title;
        if (title.trimmed().isEmpty()) {
            title = QFileInfo(song.file_path).completeBaseName();
        }

        const QString artist = song.artist.trimmed().isEmpty() ? "未知歌手" : song.artist;
        const QString album = song.album.trimmed().isEmpty() ? "-" : song.album;

        addSong(title, artist, album, song.duration_sec);
    }
}
