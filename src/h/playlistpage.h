//
// Created by edward on 2026/4/13.
//

#ifndef ECHONOTE_PLAYLISTPAGE_H
#define ECHONOTE_PLAYLISTPAGE_H
#include <QWidget>


class PlaylistPage : public QWidget {
    Q_OBJECT

public:
    explicit PlaylistPage(QWidget *parent = nullptr);

    ~PlaylistPage() override;
};


#endif //ECHONOTE_PLAYLISTPAGE_H
