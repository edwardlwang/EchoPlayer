//
// Created by edward on 2026/4/4.
//

#ifndef ECHONOTE_BOTTOMSTATUSBAR_H
#define ECHONOTE_BOTTOMSTATUSBAR_H
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include<QLabel>


class PlayerControlBar : public QWidget {
    Q_OBJECT

public:
    explicit PlayerControlBar(QWidget *parent = nullptr);

    ~PlayerControlBar() override;

private:
    QHBoxLayout *m_player_control_bar_layout;

    QWidget *m_song_info_area;
    QHBoxLayout *m_song_info_area_layout;
    QLabel *m_song_cover_label;
    QLabel *m_song_info_label;

    QWidget *m_play_area;
    QHBoxLayout *m_play_area_layout;
    QPushButton *m_play_order_btn;
    QPushButton *m_prev_btn;
    QPushButton *m_play_pause_btn;
    QPushButton *m_next_btn;
    QPushButton *m_play_list_btn;

    QWidget *m_control_area;
    QHBoxLayout *m_control_area_layout;
    QPushButton *m_volume_btn;

    struct BtnInfo {
        QString key;
        QString iconPath;
    };

    static QPushButton *createButton(const BtnInfo &btn_info, QWidget *parent = nullptr);
};


#endif //ECHONOTE_BOTTOMSTATUSBAR_H
