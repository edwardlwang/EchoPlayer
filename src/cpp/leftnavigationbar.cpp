//
// Created by edward on 2026/4/4.
//

#include "../h/leftnavigationbar.h"

LeftNavigationBar::LeftNavigationBar(QWidget *parent) : QWidget(parent) {
    qDebug() << "LeftNavigationBar构造完成";

    this->setObjectName("leftNavigationBar");
    this->setFixedWidth(108);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setAttribute(Qt::WA_StyledBackground, true);

    leftNavigationBarLayout = new QVBoxLayout(this);
    leftNavigationBarLayout->setContentsMargins(0, 0, 0, 0);
    leftNavigationBarLayout->setSpacing(0);

    left_navigation_bar_button_group = new QButtonGroup(this);
    left_navigation_bar_button_group->setExclusive(true);

    const QVector<BtnInfo> btnInfos = {
        {"songs", "歌曲", ":/icons/microphone.svg"},
        {"albums", "专辑", ":/icons/disc.svg"},
        {"artists", "艺术家", ":/icons/user.svg"},
        {"playlists", "歌单", ":/icons/queue-list.svg"},
        {"settings", "设置", ":/icons/settings.svg"}
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
        if (item.key == "songs") {
            btn->setChecked(true);
        }

        leftNavigationBarButtons.push_back(btn);
        leftNavigationBarLayout->addWidget(btn);
        left_navigation_bar_button_group->addButton(btn);

        connect(btn, &QPushButton::clicked, this, [=] {
            qDebug() << "点击了：" << btn->objectName();
            emit navigationButtonClicked(btn->objectName());
        });
    }
    leftNavigationBarLayout->addStretch(1);
}

LeftNavigationBar::~LeftNavigationBar() {
    qDebug() << "LeftNavigationBar析构完成";
}
