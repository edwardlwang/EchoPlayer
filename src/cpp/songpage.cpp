//
// Created by edward on 2026/4/12.
//

#include "../h/songpage.h"

SongPage::SongPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "SongPage构造完成";
    this->setObjectName("songpage");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

SongPage::~SongPage() {
    qDebug() << "SongPage析构完成";
}
