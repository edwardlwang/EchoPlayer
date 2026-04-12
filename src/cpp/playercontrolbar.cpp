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

    songInfoArea=new QWidget(this);
    playerControlBarLayout->addWidget(songInfoArea);
    songInfoAreaLayout=new QHBoxLayout(songInfoArea);

    playArea=new QWidget(this);
    playerControlBarLayout->addWidget(playArea);
    playAreaLayout=new QHBoxLayout(playArea);

    controlArea=new QWidget(this);
    playerControlBarLayout->addWidget(controlArea);
    controlAreaLayout=new QHBoxLayout(controlArea);

    songCoverLabel = new QLabel("封面", this);
    songCoverLabel->setFixedSize(48, 48);
    songCoverLabel->setObjectName("songCoverLabel");
    songInfoAreaLayout->addWidget(songCoverLabel);

    songInfoLabel = new QLabel("信息", this);
    songInfoLabel->setFixedSize(48, 48);
    songInfoLabel->setObjectName("songInfoLabel");
    songInfoAreaLayout->addWidget(songInfoLabel);

    playOrderBtn = new QPushButton(this);
    playOrderBtn->setFixedSize(48, 48);
    playOrderBtn->setObjectName("playOrderBtn");
    playOrderBtn->setIconSize(QSize(32,32));
    playOrderBtn->setFlat(true);
    playOrderBtn->setIcon(QIcon(":/icons/repeat.svg"));
    playAreaLayout->addWidget(playOrderBtn);

    prevBtn = new QPushButton(this);
    prevBtn->setFixedSize(48, 48);
    prevBtn->setObjectName("prevBtn");
    prevBtn->setIconSize(QSize(32, 32));
    prevBtn->setFlat(true);
    prevBtn->setIcon(QIcon(":/icons/previous.svg"));
    playAreaLayout->addWidget(prevBtn);

    playPauseBtn = new QPushButton(this);
    playPauseBtn->setFixedSize(48, 48);
    playPauseBtn->setObjectName("playPauseBtn");
    playPauseBtn->setIconSize(QSize(32, 32));
    playPauseBtn->setFlat(true);
    playPauseBtn->setIcon(QIcon(":/icons/play.svg"));
   playAreaLayout->addWidget(playPauseBtn);

    nextBtn = new QPushButton(this);
    nextBtn->setFixedSize(48, 48);
    nextBtn->setObjectName("nextBtn");
    nextBtn->setIconSize(QSize(32, 32));
    nextBtn->setFlat(true);
    nextBtn->setIcon(QIcon(":/icons/next.svg"));
    playAreaLayout->addWidget(nextBtn);

    playListBtn = new QPushButton(this);
    playListBtn->setFixedSize(48, 48);
    playListBtn->setObjectName("playListBtn");
    playListBtn->setIconSize(QSize(32, 32));
    playListBtn->setFlat(true);
    playListBtn->setIcon(QIcon(":/icons/playlist.svg"));
    playAreaLayout->addWidget(playListBtn);

    volumeBtn = new QPushButton(this);
    volumeBtn->setFixedSize(48, 48);
    volumeBtn->setObjectName("volumeBtn");
    volumeBtn->setIconSize(QSize(32, 32));
    volumeBtn->setFlat(true);
    volumeBtn->setIcon(QIcon(":/icons/volume-high.svg"));
    controlAreaLayout->addWidget(volumeBtn);
}


PlayerControlBar::~PlayerControlBar() {
    qDebug() << "PlayerControlBar析构完成";
}
