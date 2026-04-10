//
// Created by edward on 2026/4/4.
//

#include "../h/playercontrolbar.h"

PlayerControlBar::PlayerControlBar(QWidget *parent) : QWidget(parent) {
    qDebug() << "PlayerControlBar构造完成";

    this->setObjectName("playerControlBar");
    this->setFixedHeight(108);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_StyledBackground, true);

    playerControlBarLayout = new QHBoxLayout(this);
    playerControlBarLayout->setContentsMargins(0, 0, 0, 0);
    playerControlBarLayout->setSpacing(8);

    songCoverLabel = new QLabel("封面", this);
    songCoverLabel->setFixedSize(64, 64);
    songCoverLabel->setObjectName("songCoverLabel");
    playerControlBarLayout->addWidget(songCoverLabel);

    songInfoLabel = new QLabel("信息", this);
    songInfoLabel->setFixedSize(64, 64);
    songInfoLabel->setObjectName("songInfoLabel");
    playerControlBarLayout->addWidget(songInfoLabel);

    prevBtn = new QPushButton(this);
    prevBtn->setFixedSize(64, 64);
    prevBtn->setObjectName("prevBtn");
    prevBtn->setFlat(true);
    prevBtn->setIcon(QIcon(":/icons/arrow_left_icon.svg"));
    playerControlBarLayout->addWidget(prevBtn);

    playPauseBtn = new QPushButton(this);
    playPauseBtn->setFixedSize(64, 64);
    playPauseBtn->setObjectName("playPauseBtn");
    // playPauseBtn->setIconSize(QSize(32, 32));
    playPauseBtn->setFlat(true);
    playPauseBtn->setIcon(QIcon(":/icons/play_icon.svg"));
    playerControlBarLayout->addWidget(playPauseBtn);

    nextBtn = new QPushButton(this);
    nextBtn->setFixedSize(64, 64);
    nextBtn->setObjectName("nextBtn");
    nextBtn->setFlat(true);
    nextBtn->setIcon(QIcon(":/icons/arrow_right_icon.svg"));
    playerControlBarLayout->addWidget(nextBtn);

    playListBtn = new QPushButton(this);
    playListBtn->setFixedSize(64, 64);
    playListBtn->setObjectName("playListBtn");
    playListBtn->setFlat(true);
    playListBtn->setIcon(QIcon(":/icons/list_icon.svg"));
    playerControlBarLayout->addWidget(playListBtn);

    volumeBtn = new QPushButton(this);
    volumeBtn->setFixedSize(64, 64);
    volumeBtn->setObjectName("volumeBtn");
    volumeBtn->setFlat(true);
    volumeBtn->setIcon(QIcon(":/icons/volume_icon.svg"));
    playerControlBarLayout->addWidget(volumeBtn);
}


PlayerControlBar::~PlayerControlBar() {
    qDebug() << "PlayerControlBar析构完成";
}
