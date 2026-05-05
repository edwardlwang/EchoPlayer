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
    m_player_control_bar_layout->setSpacing(0);

    m_song_detail_widget = new QWidget(this);
    m_player_control_bar_layout->addWidget(m_song_detail_widget, 1);
    m_song_detail_layout = new QHBoxLayout(m_song_detail_widget);
    m_song_detail_layout->setContentsMargins(10, 0, 0, 0); // 加一点左右边距，不贴边
    m_song_detail_layout->setSpacing(0); // 封面和歌词区的间距

    m_song_info_widget = new QWidget(m_song_detail_widget);
    m_song_detail_layout->addWidget(m_song_info_widget, 0);
    // m_song_info_widget->setMinimumWidth(150); // 至少留150px宽度
    m_song_info_layout = new QHBoxLayout(m_song_info_widget);
    m_song_info_layout->setContentsMargins(0, 0, 0, 0);
    m_song_info_layout->setSpacing(10); // 封面和文字区的间距

    m_cover_btn = new QPushButton("封面", m_song_info_widget);
    m_cover_btn->setFixedSize(48, 48);
    m_cover_btn->setObjectName("coverbtn");
    m_song_info_layout->addWidget(m_cover_btn);

    m_song_meta_widget = new QWidget(m_song_info_widget);
    m_song_info_layout->addWidget(m_song_meta_widget);
    m_song_meta_layout = new QVBoxLayout(m_song_meta_widget);

    m_singer_label = new QLabel("歌手", m_song_meta_widget);
    m_song_meta_layout->addWidget(m_singer_label);

    m_album_label = new QLabel("专辑", m_song_meta_widget);
    m_song_meta_layout->addWidget(m_album_label);

    m_lyrics_widget = new QWidget(m_song_detail_widget);
    m_song_detail_layout->addWidget(m_lyrics_widget, 1);
    m_lyrics_layout = new QVBoxLayout(m_lyrics_widget);

    m_lyrics_label = new QLabel("歌词", m_lyrics_widget);
    m_lyrics_layout->addWidget(m_lyrics_label, 0, Qt::AlignCenter);

    m_lyrics_cn_label = new QLabel("歌词翻译", m_lyrics_widget);
    m_lyrics_layout->addWidget(m_lyrics_cn_label, 0, Qt::AlignCenter);

    m_control_widget = new QWidget(this);
    m_player_control_bar_layout->addWidget(m_control_widget, 0);
    m_control_widget_layout = new QHBoxLayout(m_control_widget);
    m_control_widget_layout->setContentsMargins(0, 0, 50, 0);
    m_control_widget_layout->setSpacing(10);

    const QVector<BtnInfo> btn_infos = {
        {"playorderbtn", "", ":/icons/repeat.svg"},
        {"prevbtn", "", ":/icons/previous.svg"},
        {"playpausebtn", "", ":/icons/play.svg"},
        {"nextbtn", "", ":/icons/next.svg"},
        {"playlistbtn", "", ":/icons/playlist.svg"},
        {"volumebtn", "", ":/icons/volume-high.svg"}
    };

    m_play_order_btn = createButton(btn_infos[0], this);
    m_control_widget_layout->addWidget(m_play_order_btn);
    m_prev_btn = createButton(btn_infos[1], this);
    m_control_widget_layout->addWidget(m_prev_btn);

    m_play_pause_btn = createButton(btn_infos[2], this);
    m_control_widget_layout->addWidget(m_play_pause_btn);

    m_next_btn = createButton(btn_infos[3], this);
    m_control_widget_layout->addWidget(m_next_btn);

    m_play_list_btn = createButton(btn_infos[4], this);
    m_control_widget_layout->addWidget(m_play_list_btn);

    m_volume_btn = createButton(btn_infos[5], this);
    m_control_widget_layout->addWidget(m_volume_btn);

    connect(m_play_pause_btn, &QPushButton::clicked, this, &PlayerControlBar::playPauseClicked);
    connect(m_next_btn, &QPushButton::clicked, this, &PlayerControlBar::nextClicked);
    connect(m_prev_btn, &QPushButton::clicked, this, &PlayerControlBar::prevClicked);
    connect(m_play_list_btn, &QPushButton::clicked, this, &PlayerControlBar::playlistClicked);
    connect(m_play_order_btn, &QPushButton::clicked, this, &PlayerControlBar::playOrderClicked);
    connect(m_volume_btn, &QPushButton::clicked, this, &PlayerControlBar::volumeClicked);
}


PlayerControlBar::~PlayerControlBar() {
    qDebug() << "PlayerControlBar析构完成";
}

void PlayerControlBar::updateSongInfo(const SongInfo &song) {
    // m_cover_btn->set
}

QPushButton *PlayerControlBar::createButton(const BtnInfo &btn_info, QWidget *parent) {
    auto *btn = new QPushButton(parent);
    btn->setFixedSize(32, 32);
    btn->setObjectName(btn_info.key);
    btn->setIconSize(QSize(32, 32));
    btn->setFlat(true);
    btn->setIcon(QIcon(btn_info.iconPath));
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet("QPushButton { padding: 0; margin: 0px; border: none; }");
    return btn;
}


