//
// Created by edward on 2026/4/12.
//

#include "../h/albumpage.h"

AlbumPage::AlbumPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "AlbumPage构造完成";
    this->setObjectName("albumpage");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

AlbumPage::~AlbumPage() {
    qDebug() << "AlbumPage析构完成";
}
