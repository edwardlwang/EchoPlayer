//
// Created by edward on 2026/4/10.
//

#include "../h/rightcontent.h"

RightContent::RightContent(QWidget *parent) : QStackedWidget(parent) {
    qDebug() << "RightContent构造完成";
    this->setObjectName("rightContent");
    this->setAttribute(Qt::WA_StyledBackground, true);

    this->addWidget(new SongWidget(this));
    this->addWidget(new AlbumWidget(this));
}

RightContent::~RightContent() {
    qDebug() << "RightContent析构完成";
}

void RightContent::showPage(const QString &key) {
    if (key == "songs") {
        this->setCurrentIndex(0);
    } else if (key == "albums") {
        this->setCurrentIndex(1);
    } else {
        qDebug() << "未知页面：" << key;
    }
}