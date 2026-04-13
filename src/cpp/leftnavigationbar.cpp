//
// Created by edward on 2026/4/4.
//

#include "../h/leftnavigationbar.h"
#include <QFrame>

LeftNavigationBar::LeftNavigationBar(QWidget *parent) : QWidget(parent) {
    qDebug() << "LeftNavigationBar构造完成";

    this->setObjectName("leftNavigationBar");
    this->setFixedWidth(108);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setAttribute(Qt::WA_StyledBackground, true);

    leftNavigationBarLayout = new QVBoxLayout(this);
    leftNavigationBarLayout->setContentsMargins(0, 0, 0, 0);
    leftNavigationBarLayout->setSpacing(0);

    const QVector<BtnInfo> btnInfos = {
        {"songs", "歌曲",   ":/icons/microphone.svg"},
        {"albums","专辑",   ":/icons/disc.svg"},
        {"artists","艺术家",":/icons/user.svg"},
        {"playlists","歌单",":/icons/queue-list.svg"},
        {"settings","设置", ":/icons/settings.svg"}
    };

    for (const BtnInfo &item: btnInfos) {
        auto *btn = new QPushButton(item.text, this);
        btn->setObjectName(item.key);
        btn->setCheckable(true);
        btn->setFlat(true);
        btn->setFixedSize(108, 60);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(QIcon(item.iconPath));
        btn->setCursor(Qt::PointingHandCursor);
        leftNavigationBarButtons.push_back(btn);
        leftNavigationBarLayout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [=] {
            qDebug() << "点击了：" << item.text;
            emit navigationButtonClicked(item.key);
        });
    }
    leftNavigationBarLayout->addStretch(1);
}

LeftNavigationBar::~LeftNavigationBar() {
    qDebug() << "LeftNavigationBar析构完成";
}
