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

    texts = {
        "歌曲", "专辑", "艺术家",
        "歌单", "设置"
    };

    for (const QString &text: texts) {
        auto *btn = new QPushButton(text, this);
        btn->setObjectName(text);
        btn->setCheckable(true);
        btn->setFlat(true);
        btn->setFixedSize(108, 60);
        btn->setIconSize(QSize(32, 32));
        leftNavigationBarButtons.push_back(btn);
        leftNavigationBarLayout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [=] { qDebug() << "点击了：" << text; });
        auto *frame = new QFrame(this);
        frame->setFrameShape(QFrame::HLine);
        frame->setFixedHeight(1);
        leftNavigationBarLayout->addWidget(frame);
    }
    leftNavigationBarLayout->addStretch(1);
}

LeftNavigationBar::~LeftNavigationBar() {
    qDebug() << "LeftNavigationBar析构完成";
}
