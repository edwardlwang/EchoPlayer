//
// Created by edward on 2026/4/4.
//

#include "../h/navigationbar.h"

NavigationBar::NavigationBar(QWidget *parent) : QWidget(parent) {
    qDebug() << "NavigationBar构造完成";

    this->setObjectName("navigationbar");
    this->setFixedWidth(144);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setAttribute(Qt::WA_StyledBackground, true);

    m_navigation_bar_layout = new QVBoxLayout(this);
    m_navigation_bar_layout->setContentsMargins(0, 0, 0, 0);
    m_navigation_bar_layout->setSpacing(0);

    m_navigation_bar_button_group = new QButtonGroup(this);
    m_navigation_bar_button_group->setExclusive(true);

    btnInfos = {
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
        // btn->setFixedSize(108, 60);
        btn->setFixedHeight(60);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(QIcon(item.icon_path));
        btn->setText(item.text);
        btn->setStyleSheet(
        "QPushButton {"
                "text-align: center;"      // 文字水平居中（不控制图标位置）
            "}"
        );

        btn->setCursor(Qt::PointingHandCursor);
        if (item.key == "songs") {
            btn->setChecked(true);
        }

        m_navigation_bar_buttons.push_back(btn);
        m_navigation_bar_layout->addWidget(btn);
        m_navigation_bar_button_group->addButton(btn);

        connect(btn, &QPushButton::clicked, this, [=] {
            qDebug() << "点击了：" << btn->objectName();
            emit navigationButtonClicked(btn->objectName());
        });
    }
    m_navigation_bar_layout->addStretch(1);
}

NavigationBar::~NavigationBar() {
    qDebug() << "NavigationBar析构完成";
}
