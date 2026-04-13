//
// Created by edward on 2026/4/13.
//

#include "../h/playlistpage.h"

PlaylistPage::PlaylistPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "PlaylistPage构造完成";
    this->setObjectName("playlistpage");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

PlaylistPage::~PlaylistPage() {
    qDebug() << "PlaylistPage析构完成";
}
