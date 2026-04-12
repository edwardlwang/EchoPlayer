//
// Created by edward on 2026/4/12.
//

#include "../h/songwidget.h"

SongWidget::SongWidget(QWidget *parent) {
    qDebug() << "SongWidget构造完成";
    this->setObjectName("songWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

SongWidget::~SongWidget() {
    qDebug() << "SongWidget析构完成";
}
