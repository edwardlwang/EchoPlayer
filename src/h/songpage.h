//
// Created by edward on 2026/4/12.
//

#ifndef ECHONOTE_SONGPAGE_H
#define ECHONOTE_SONGPAGE_H

#include <QTableWidget>
#include <QVBoxLayout>

#include "commonstruct.h"

namespace ScanMusic {
    class ScannerService;
}

class SongPage : public QWidget {
    Q_OBJECT

public:
    explicit SongPage(QWidget *parent = nullptr);

    ~SongPage() override;

    void addSong(const QImage &cover, const QString &title, const QString &artist, const QString &album,
                 const int &duration_sec) const;

    void setSongs(const QVector<SongInfo> &songs);

    const QVector<SongInfo> &getSongs() const;

signals:
    void songActivated(const SongInfo &song, int row);

private:
    QVBoxLayout *m_main_layout = nullptr; // 垂直布局
    QTableWidget *m_song_table = nullptr; // 歌曲列表
    QVector<SongInfo> m_songs; // 存放所有歌曲
    ScanMusic::ScannerService *m_scanner = nullptr;

    // 异步批量回调时，增量追加到表格，避免每批都全量重绘。
    void appendSongsBatch(const QVector<SongInfo> &songs);
};


#endif //ECHONOTE_SONGPAGE_H
