//
// Created by edward on 2026/4/10.
//

#ifndef ECHONOTE_RIGHTCONTENT_H
#define ECHONOTE_RIGHTCONTENT_H

#include <QStackedWidget>

class RightContent : public QStackedWidget {
    Q_OBJECT

public:
    RightContent(QWidget *parent = nullptr);

    ~RightContent();

private:
};


#endif //ECHONOTE_RIGHTCONTENT_H
