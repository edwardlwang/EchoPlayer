//
// Created by edward on 2026/4/12.
//

#include "../h/songpage.h"
#include <QHeaderView>


SongPage::SongPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "SongPage构造完成";
    this->setObjectName("songpage");
    this->setAttribute(Qt::WA_StyledBackground, true);

    m_main_layout = new QVBoxLayout(this);
    m_main_layout->setContentsMargins(0, 0, 0, 0);
    m_main_layout->setSpacing(0);
    this->setLayout(m_main_layout);

    m_song_table = new QTableWidget(this);
    m_song_table->setColumnCount(4);
    m_song_table->setHorizontalHeaderLabels({"歌曲", "歌手", "专辑", "时长"});
    m_song_table->horizontalHeader()->setStretchLastSection(true);
    m_song_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_song_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_main_layout->addWidget(m_song_table);
}

SongPage::~SongPage() {
    qDebug() << "SongPage析构完成";
}

void SongPage::addSong(const QString &title, const QString &artist, const QString &album, const int &duration_sec) {
    const int row = m_song_table->rowCount();
    m_song_table->insertRow(row);

    const int min = duration_sec / 60;
    const int sec = duration_sec % 60;

    const QString duration = QString("%1:%2")
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0'));

    m_song_table->setItem(row, 0, new QTableWidgetItem(title));
    m_song_table->setItem(row, 1, new QTableWidgetItem(artist));
    m_song_table->setItem(row, 2, new QTableWidgetItem(album));
    m_song_table->setItem(row, 3, new QTableWidgetItem(duration));
}
