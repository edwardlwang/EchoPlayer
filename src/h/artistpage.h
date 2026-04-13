//
// Created by edward on 2026/4/13.
//

#ifndef ECHONOTE_SINGERPAGE_H
#define ECHONOTE_SINGERPAGE_H

#include <QWidget>

class ArtistPage : public QWidget {
    Q_OBJECT

public:
    explicit ArtistPage(QWidget *parent = nullptr);

    ~ArtistPage() override;
};


#endif //ECHONOTE_SINGERPAGE_H
