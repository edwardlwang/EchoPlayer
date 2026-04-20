//
// Created by edward on 2026/4/4.
//

#include "../h/playercontrolbar.h"

PlayerControlBar::PlayerControlBar(QWidget *parent) : QWidget(parent) {
    qDebug() << "PlayerControlBar构造完成";

    this->setObjectName("playercontrolbar");
    this->setFixedHeight(81);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_StyledBackground, true);

    m_player_control_bar_layout = new QHBoxLayout(this);
    m_player_control_bar_layout->setContentsMargins(0, 0, 0, 0);
    m_player_control_bar_layout->setSpacing(8);

    m_song_info_area = new QWidget(this);
    m_player_control_bar_layout->addWidget(m_song_info_area);
    m_song_info_area_layout = new QHBoxLayout(m_song_info_area);

    m_play_area = new QWidget(this);
    m_player_control_bar_layout->addWidget(m_play_area);
    m_play_area_layout = new QHBoxLayout(m_play_area);

    m_control_area = new QWidget(this);
    m_player_control_bar_layout->addWidget(m_control_area);
    m_control_area_layout = new QHBoxLayout(m_control_area);

    m_song_cover_label = new QLabel("封面", this);
    m_song_cover_label->setFixedSize(48, 48);
    m_song_cover_label->setObjectName("songcoverlabel");
    m_song_info_area_layout->addWidget(m_song_cover_label, 0, Qt::AlignLeft);

    m_song_info_label = new QLabel("信息", this);
    m_song_info_label->setFixedSize(48, 48);
    m_song_info_label->setObjectName("songinfolabel");
    m_song_info_area_layout->addWidget(m_song_info_label, 0, Qt::AlignLeft);

    const QVector<BtnInfo> btn_infos = {
        {"playorderbtn", "", ":/icons/repeat.svg"},
        {"prevbtn", "", ":/icons/previous.svg"},
        {"playpausebtn", "", ":/icons/play.svg"},
        {"nextbtn", "", ":/icons/next.svg"},
        {"playlistbtn", "", ":/icons/playlist.svg"},
        {"volumebtn", "", ":/icons/volume-high.svg"}
    };

    m_play_order_btn = createButton(btn_infos[0], this);
    m_play_area_layout->addWidget(m_play_order_btn);
    m_prev_btn = createButton(btn_infos[1], this);
    m_play_area_layout->addWidget(m_prev_btn);

    m_play_pause_btn = createButton(btn_infos[2], this);
    m_play_area_layout->addWidget(m_play_pause_btn);


    m_next_btn = createButton(btn_infos[3], this);
    m_play_area_layout->addWidget(m_next_btn);

    m_play_list_btn = createButton(btn_infos[4], this);
    m_play_area_layout->addWidget(m_play_list_btn);

    m_volume_btn = createButton(btn_infos[5], this);
    m_control_area_layout->addWidget(m_volume_btn, 0, Qt::AlignRight);
}

PlayerControlBar::~PlayerControlBar() {
    qDebug() << "PlayerControlBar析构完成";
}

QPushButton *PlayerControlBar::createButton(const BtnInfo &btn_info, QWidget *parent) {
    auto *btn = new QPushButton(parent);
    btn->setFixedSize(48, 48);
    btn->setObjectName(btn_info.key);
    btn->setIconSize(QSize(32, 32));
    btn->setFlat(true);
    btn->setIcon(QIcon(btn_info.iconPath));
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}
