//
// Created by edward on 2026/4/1.
//

#ifndef MUSICPLAYER_LEFTTOOLBAR_H
#define MUSICPLAYER_LEFTTOOLBAR_H
#include <QToolBar>
#include <QPushButton>
#include <iostream>


class LeftToolBar : public QToolBar {
    Q_OBJECT

public:
    LeftToolBar();

    ~LeftToolBar() override;

private slots:
private:
    std::vector<QPushButton *> leftToolBarButtons;
};


#endif //MUSICPLAYER_LEFTTOOLBAR_H
