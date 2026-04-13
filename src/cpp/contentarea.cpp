//
// Created by edward on 2026/4/10.
//

#include "../h/contentarea.h"

ContentArea::ContentArea(QWidget *parent) : QStackedWidget(parent) {
    qDebug() << "ContentArea构造完成";
    this->setObjectName("contentarea");
    this->setAttribute(Qt::WA_StyledBackground, true);

    m_page_index_map.insert("songs", this->addWidget(new SongPage(this)));
    m_page_index_map.insert("albums", this->addWidget(new AlbumPage(this)));
    m_page_index_map.insert("artists", this->addWidget(new ArtistPage(this)));
    m_page_index_map.insert("playlists", this->addWidget(new PlaylistPage(this)));
    m_page_index_map.insert("settings", this->addWidget(new SettingPage(this)));
}

ContentArea::~ContentArea() {
    qDebug() << "ContentArea析构完成";
}

void ContentArea::showPage(const QString &key) {
    if (!m_page_index_map.contains(key)) {
        qDebug() << "未知页面：" << key;
        return;
    }
    setCurrentIndex(m_page_index_map.value(key));
}
