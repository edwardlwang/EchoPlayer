//
// Created by edward on 2026/4/12.
//

#include "../h/albumwidget.h"

AlbumWidget::AlbumWidget(QWidget *parent) {
    qDebug() << "AlbumWidget构造完成";
    this->setObjectName("albumWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

AlbumWidget::~AlbumWidget() {
    qDebug() << "AlbumWidget析构完成";
}
