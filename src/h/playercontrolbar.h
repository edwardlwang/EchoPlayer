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
    QLabel *songCoverLabel;
    QLabel *songInfoLabel;
    QPushButton *prevBtn;
    QPushButton *playPauseBtn;
    QPushButton *nextBtn;
    QPushButton *playListBtn;
    QPushButton *volumeBtn;
};


#endif //ECHONOTE_BOTTOMSTATUSBAR_H
