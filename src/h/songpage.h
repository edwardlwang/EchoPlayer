//
// Created by edward on 2026/4/12.
//

#ifndef ECHONOTE_SONGPAGE_H
#define ECHONOTE_SONGPAGE_H

#include <QVBoxLayout>
#include <QTableWidget>


class SongPage : public QWidget {
    Q_OBJECT

public:
    explicit SongPage(QWidget *parent = nullptr);

    ~SongPage() override;

    void addSong(const QString &title, const QString &artist, const QString &album, const int &duration_sec);

private:
    QVBoxLayout *m_main_layout;
    QTableWidget *m_song_table;
};


#endif //ECHONOTE_SONGPAGE_H
