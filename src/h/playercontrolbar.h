//
// Created by edward on 2026/4/4.
//

#ifndef ECHONOTE_BOTTOMSTATUSBAR_H
#define ECHONOTE_BOTTOMSTATUSBAR_H
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include<QLabel>


class PlayerControlBar : public QWidget {
    Q_OBJECT

public:
    PlayerControlBar(QWidget *parent = nullptr);

    ~PlayerControlBar() override;

private:
    QHBoxLayout *playerControlBarLayout;

    QWidget *songInfoArea;
    QHBoxLayout *songInfoAreaLayout;
    QLabel *songCoverLabel;
    QLabel *songInfoLabel;

    QWidget *playArea;
    QHBoxLayout *playAreaLayout;
    QPushButton *playOrderBtn;
    QPushButton *prevBtn;
    QPushButton *playPauseBtn;
    QPushButton *nextBtn;
    QPushButton *playListBtn;

    QWidget *controlArea;
    QHBoxLayout *controlAreaLayout;
    QPushButton *volumeBtn;
};


#endif //ECHONOTE_BOTTOMSTATUSBAR_H
