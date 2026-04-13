//
// Created by edward on 2026/4/13.
//

#include "../h/artistpage.h"

ArtistPage::ArtistPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "ArtistPage构造完成";
    this->setObjectName("artistpage");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

ArtistPage::~ArtistPage() {
    qDebug() << "ArtistPage析构完成";
}
