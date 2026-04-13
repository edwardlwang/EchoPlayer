//
// Created by edward on 2026/4/4.
//

#include "../h/playercontrolbar.h"

PlayerControlBar::PlayerControlBar(QWidget *parent) : QWidget(parent) {
    qDebug() << "PlayerControlBar构造完成";

    this->setObjectName("playerControlBar");
    this->setFixedHeight(81);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_StyledBackground, true);

    playerControlBarLayout = new QHBoxLayout(this);
    playerControlBarLayout->setContentsMargins(0, 0, 0, 0);
    playerControlBarLayout->setSpacing(8);

    songInfoArea = new QWidget(this);
    playerControlBarLayout->addWidget(songInfoArea);
    songInfoAreaLayout = new QHBoxLayout(songInfoArea);

    playArea = new QWidget(this);
    playerControlBarLayout->addWidget(playArea);
    playAreaLayout = new QHBoxLayout(playArea);

    controlArea = new QWidget(this);
    playerControlBarLayout->addWidget(controlArea);
    controlAreaLayout = new QHBoxLayout(controlArea);

    songCoverLabel = new QLabel("封面", this);
    songCoverLabel->setFixedSize(48, 48);
    songCoverLabel->setObjectName("songCoverLabel");
    songInfoAreaLayout->addWidget(songCoverLabel, 0, Qt::AlignLeft);

    songInfoLabel = new QLabel("信息", this);
    songInfoLabel->setFixedSize(48, 48);
    songInfoLabel->setObjectName("songInfoLabel");
    songInfoAreaLayout->addWidget(songInfoLabel, 0, Qt::AlignLeft);

    const QVector<BtnInfo> btn_infos = {
        {"playorderbtn", ":/icons/repeat.svg"},
        {"prevbtn", ":/icons/previous.svg"},
        {"playpausebtn", ":/icons/play.svg"},
        {"nextbtn", ":/icons/next.svg"},
        {"playListbtn", ":/icons/playlist.svg"},
        {"volumeBtn", ":/icons/volume-high.svg"}
    };

    playOrderBtn = createButton(btn_infos[0], this);
    playAreaLayout->addWidget(playOrderBtn);
    prevBtn = createButton(btn_infos[1], this);
    playAreaLayout->addWidget(prevBtn);

    playPauseBtn = createButton(btn_infos[2], this);
    playAreaLayout->addWidget(playPauseBtn);


    nextBtn = createButton(btn_infos[3], this);
    playAreaLayout->addWidget(nextBtn);

    playListBtn = createButton(btn_infos[4], this);
    playAreaLayout->addWidget(playListBtn);

    volumeBtn = createButton(btn_infos[5], this);
    controlAreaLayout->addWidget(volumeBtn, 0, Qt::AlignRight);
}

PlayerControlBar::~PlayerControlBar() {
    qDebug() << "PlayerControlBar析构完成";
}

QPushButton *PlayerControlBar::createButton(const BtnInfo &item, QWidget *parent) {
    auto *btn = new QPushButton(parent);
    btn->setFixedSize(48, 48);
    btn->setObjectName(item.key);
    btn->setIconSize(QSize(32, 32));
    btn->setFlat(true);
    btn->setIcon(QIcon(item.iconPath));
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}
