//
// Created by edward on 2026/4/10.
//

#include "../h/rightcontent.h"

RightContent::RightContent(QWidget *parent) : QStackedWidget(parent) {
    qDebug() << "RightContent构造完成";
    this->setObjectName("rightcontent");
    this->setAttribute(Qt::WA_StyledBackground, true);

    pageIndexMap.insert("songs", this->addWidget(new SongPage(this)));
    pageIndexMap.insert("albums", this->addWidget(new AlbumPage(this)));
    pageIndexMap.insert("artists", this->addWidget(new ArtistPage(this)));
    pageIndexMap.insert("playlists", this->addWidget(new PlaylistPage(this)));
    pageIndexMap.insert("settings", this->addWidget(new SettingPage(this)));
}

RightContent::~RightContent() {
    qDebug() << "RightContent析构完成";
}

void RightContent::showPage(const QString &key) {
    if (!pageIndexMap.contains(key)) {
        qDebug() << "未知页面：" << key;
        return;
    }
    setCurrentIndex(pageIndexMap.value(key));
}
