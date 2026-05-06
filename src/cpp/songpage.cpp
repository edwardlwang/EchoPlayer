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

    // 1) 去掉单元格网格线
    m_song_table->setShowGrid(false);

    // 2) 去掉外框
    m_song_table->setFrameShape(QFrame::NoFrame);

    // 3) 不显示行号列（左侧竖表头）
    m_song_table->verticalHeader()->setVisible(false);

    // 不显示顶部表头
    m_song_table->horizontalHeader()->setVisible(false);

    m_song_table->setColumnCount(5);
    m_song_table->setHorizontalHeaderLabels({"封面", "歌曲", "歌手", "专辑", "时长"});
    // m_song_table->horizontalHeader()->setStretchLastSection(true); // 最后一列吃满
    auto *horizontal_header = m_song_table->horizontalHeader();
    horizontal_header->setStretchLastSection(false); // 避免最后一列自动吃满
    horizontal_header->setSectionResizeMode(0, QHeaderView::Fixed);
    m_song_table->setColumnWidth(0, 48);
    m_song_table->verticalHeader()->setDefaultSectionSize(48);
    horizontal_header->setSectionResizeMode(1, QHeaderView::Stretch); // 标题列尽量占空间
    horizontal_header->setSectionResizeMode(2, QHeaderView::ResizeToContents); // 其他列按内容
    horizontal_header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    horizontal_header->setSectionResizeMode(4, QHeaderView::Fixed); // 时长列固定宽度
    // m_song_table->setColumnWidth(3, 72
    m_song_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_song_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_main_layout->addWidget(m_song_table);

    connect(m_song_table, &QTableWidget::cellDoubleClicked, this, [this](int row, int) {
        if (row >= 0 && row < m_songs.size()) {
            emit songActivated(m_songs[row], row);
        }
    });

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

void SongPage::addSong(const QPixmap &cover, const QString &title, const QString &artist, const QString &album,
                       const int &duration_sec)
const {
    const int row = m_song_table->rowCount();
    m_song_table->insertRow(row);

    const int min = duration_sec / 60;
    const int sec = duration_sec % 60;

    const QString duration = QString("%1:%2")
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0'));

    // addSong 只负责“渲染一行”，不做扫描/刷新，避免递归调用。
    auto *cover_item = new QTableWidgetItem();
    QPixmap scaled_cover = cover.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    cover_item->setData(Qt::DecorationRole, scaled_cover);
    // cover_item->setIcon(QIcon(cover));
    cover_item->setSizeHint(QSize(48, 48));

    m_song_table->setItem(row, 0, cover_item);
    m_song_table->setItem(row, 1, new QTableWidgetItem(title));
    m_song_table->setItem(row, 2, new QTableWidgetItem(artist));
    m_song_table->setItem(row, 3, new QTableWidgetItem(album));
    m_song_table->setItem(row, 4, new QTableWidgetItem(duration));
}

void SongPage::setSongs(const QVector<SongInfo> &songs) {
    m_songs = songs;
    m_song_table->setRowCount(0);

    // setSongs 负责批量刷新表格。
    for (const SongInfo &song: m_songs) {
        QPixmap cover = song.cover;
        if (cover.isNull()) {
            cover = QPixmap(":/imgs/album.png");
        }

        QString title = song.title;

        if (title.trimmed().isEmpty()) {
            title = QFileInfo(song.file_path).completeBaseName();
        }

        const QString artist = song.artist.trimmed().isEmpty() ? "未知歌手" : song.artist;
        const QString album = song.album.trimmed().isEmpty() ? "-" : song.album;

        addSong(cover, title, artist, album, song.duration_sec);
    }

    qDebug() << "SongPage loaded songs:" << m_songs.size();
}

const QVector<SongInfo> &SongPage::getSongs() const {
    return m_songs;
}

void SongPage::appendSongsBatch(const QVector<SongInfo> &songs) {
    for (const SongInfo &song: songs) {
        m_songs.push_back(song);

        QPixmap cover = song.cover;
        if (cover.isNull()) {
            cover = QPixmap(":/imgs/album.png");
        }

        QString title = song.title;
        if (title.trimmed().isEmpty()) {
            title = QFileInfo(song.file_path).completeBaseName();
        }

        const QString artist = song.artist.trimmed().isEmpty() ? "未知歌手" : song.artist;
        const QString album = song.album.trimmed().isEmpty() ? "-" : song.album;

        addSong(cover, title, artist,
                album, song.duration_sec);
    }
}
