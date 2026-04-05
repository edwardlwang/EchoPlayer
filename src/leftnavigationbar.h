//
// Created by edward on 2026/4/4.
//

#ifndef ECHONOTE_LEFTNAVIGATIONBAR_H
#define ECHONOTE_LEFTNAVIGATIONBAR_H

#include <vector>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStringList>

class LeftNavigationBar : public QWidget {
    Q_OBJECT

public:
    LeftNavigationBar(QWidget *parent = nullptr);

    ~LeftNavigationBar() override;

private:
    QVBoxLayout *leftNavigationBarLayout;
    QStringList texts;
    std::vector<QPushButton *> leftNavigationBarButtons;
};


#endif //ECHONOTE_LEFTNAVIGATIONBAR_H
