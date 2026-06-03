//
// Created by edward on 2026/4/4.
//

#ifndef ECHONOTE_BOTTOMSTATUSBAR_H
#define ECHONOTE_BOTTOMSTATUSBAR_H
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <qslider.h>
#include <QTableWidget>

#include "commonstruct.h"


class PlayerControlBar : public QWidget {
    Q_OBJECT

public:
    explicit PlayerControlBar(QWidget *parent = nullptr);

    ~PlayerControlBar() override;

    void updateSongInfo(const SongInfo &song) const;
    void updatePlaylist(const QVector<SongInfo> &queue, int currentIndex);

    void updatePlayerStatusIcon(bool is_playing) const;

    QSlider *getVolumeSlider() const;

signals:
    void playPauseClicked();

    void nextClicked();

    void prevClicked();

    void playlistClicked();

    void playOrderClicked();

    void volumeClicked();

private:
    QHBoxLayout *m_player_control_bar_layout;

    QWidget *m_song_detail_widget; // 歌曲详情面板（封面+信息+歌词）
    QHBoxLayout *m_song_detail_layout;

    QWidget *m_song_info_widget; // 歌曲基础信息区
    QHBoxLayout *m_song_info_layout;

    QPushButton *m_cover_btn;

    QWidget *m_song_meta_widget; // 歌曲元数据（歌手+专辑）
    QVBoxLayout *m_song_meta_layout;
    QLabel *m_title_label;
    QLabel *m_artist_album_label;

    QWidget *m_lyrics_widget;
    QVBoxLayout *m_lyrics_layout;

    QLabel *m_lyrics_label;
    QLabel *m_lyrics_cn_label;

    QWidget *m_control_widget;
    QHBoxLayout *m_control_widget_layout;


    QPushButton *m_play_order_btn;
    QPushButton *m_prev_btn;
    QPushButton *m_play_pause_btn;
    QPushButton *m_next_btn;
    QPushButton *m_playlist_btn;

    QPushButton *m_volume_btn;
    QWidget *m_volume_popup_widget;
    QVBoxLayout *m_volume_popup_layout;
    QSlider *m_volume_slider;
    QLabel *m_volume_label;

    QWidget *m_playlist_popup_widget;
    QVBoxLayout *m_playlist_popup_layout;
    QTableWidget *m_playlist_table;


    static QPushButton *createButton(const BtnInfo &btn_info, QWidget *parent = nullptr);
};


#endif //ECHONOTE_BOTTOMSTATUSBAR_H
