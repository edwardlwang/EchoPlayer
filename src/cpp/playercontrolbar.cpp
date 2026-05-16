//
// Created by edward on 2026/4/4.
//

#include "../h/playercontrolbar.h"
#include "../h/playercontroller.h"

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
    m_song_info_layout = new QHBoxLayout(m_song_info_widget);
    m_song_info_layout->setContentsMargins(0, 0, 0, 0);
    m_song_info_layout->setSpacing(10); // 封面和文字区的间距

    m_cover_btn = new QPushButton(m_song_info_widget);
    m_cover_btn->setFixedSize(48, 48);
    m_cover_btn->setIcon(QIcon(":/imgs/album.png"));
    m_cover_btn->setObjectName("coverbtn");
    m_cover_btn->setFlat(true);
    m_song_info_layout->addWidget(m_cover_btn);

    m_song_meta_widget = new QWidget(m_song_info_widget);
    m_song_info_layout->addWidget(m_song_meta_widget);
    m_song_meta_layout = new QVBoxLayout(m_song_meta_widget);

    m_title_label = new QLabel("歌名", m_song_meta_widget);
    m_song_meta_layout->addWidget(m_title_label);

    m_artist_album_label = new QLabel("歌手/专辑", m_song_meta_widget);
    m_song_meta_layout->addWidget(m_artist_album_label);

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

    m_popup = new QWidget(this, Qt::Popup);
    m_popup_layout = new QVBoxLayout(m_popup);
    m_popup_layout->setContentsMargins(5, 5, 5, 5); // 给滑块加边距，不贴死边框

    m_volume_slider = new QSlider(Qt::Vertical);
    m_volume_slider->setRange(0, 100);
    m_volume_slider->setFixedHeight(100); // 固定高度，也可根据需求调整
    m_volume_slider->setFixedWidth(30); // 固定宽度，美观
    m_volume_slider->setValue(66); // 初始音量 50%
    m_popup_layout->addWidget(m_volume_slider, 0, Qt::AlignCenter);
    m_popup->adjustSize();
    m_popup->hide();

    connect(m_volume_btn, &QPushButton::clicked, this, [this]() {
        if (m_popup->isVisible()) {
            m_popup->hide();
        } else {
            // 1. 计算按钮的中心全局坐标（水平居中用）
            QPoint btn_center = m_volume_btn->mapToGlobal(m_volume_btn->rect().center());
            // 2. 计算按钮的顶部全局坐标（垂直对齐用）
            QPoint btn_top_left = m_volume_btn->mapToGlobal(QPoint(0, 0));

            // 3. 正确位置：底部贴按钮顶部，水平和按钮中心对齐
            int popup_x = btn_center.x() - m_popup->width() / 2;
            int popup_y = btn_top_left.y() - m_popup->height();

            m_popup->move(popup_x, popup_y - (m_control_widget->height() - m_volume_btn->height()) / 2);
            m_popup->show();
            m_volume_slider->setFocus();
        }
    });

    // 播放/暂停按钮：触发实际的播放/暂停控制逻辑 同一按钮同时也更新按钮上的图标（播放/暂停样式），保持界面状态同步
    connect(m_play_pause_btn, &QPushButton::clicked, this, [this]()-> void {
        // 这里直接发出信号，由业务层处理播放/暂停逻辑
        emit playPauseClicked();
    });

    // 下一首按钮：通知业务层切换到下一首曲目
    connect(m_next_btn, &QPushButton::clicked, this, &PlayerControlBar::nextClicked);
    // 上一首按钮：通知业务层切换到上一首曲目
    connect(m_prev_btn, &QPushButton::clicked, this, &PlayerControlBar::prevClicked);

    // 播放列表按钮：打开/关闭或显示播放列表界面
    // connect(m_play_list_btn, &QPushButton::clicked, this, &PlayerControlBar::playlistClicked);

    // 播放顺序按钮：切换播放模式（如单曲循环、列表循环、随机播放等）
    // connect(m_play_order_btn, &QPushButton::clicked, this, &PlayerControlBar::playOrderClicked);

    // 音量按钮：调出音量控制（如弹出滑块、静音切换等）
    // connect(m_volume_btn, &QPushButton::clicked, this, &PlayerControlBar::volumeClicked);
}


PlayerControlBar::~PlayerControlBar() {
    qDebug() << "PlayerControlBar析构完成";
}

void PlayerControlBar::updateSongInfo(const SongInfo &song) const {
    m_cover_btn->setIcon(QIcon(QPixmap::fromImage(song.cover)));
    m_cover_btn->setIconSize(QSize(48, 48));
    m_title_label->setText(song.title);
    m_artist_album_label->setText(song.artist + " - " + song.album);
}

void PlayerControlBar::updatePlaybackStatusIcon(bool is_playing) const {
    if (is_playing) {
        m_play_pause_btn->setIcon(QIcon(":/icons/pause.svg"));
    } else {
        m_play_pause_btn->setIcon(QIcon(":/icons/play.svg"));
    }
}

QPushButton *PlayerControlBar::createButton(const BtnInfo &btn_info, QWidget *parent) {
    auto *btn = new QPushButton(parent);
    btn->setFixedSize(32, 32);
    btn->setObjectName(btn_info.key);
    btn->setIconSize(QSize(32, 32));
    btn->setFlat(true);
    btn->setIcon(QIcon(btn_info.icon_path));
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(
        "QPushButton { padding: 0; margin: 0px; border: none; }"
        "QPushButton:hover { background-color: grey; border-radius: 4px; }"
        "QPushButton:pressed { background-color: darkgray; }"
    );
    return btn;
}

QSlider *PlayerControlBar::getVolumeSlider() const {
    return m_volume_slider;
}
