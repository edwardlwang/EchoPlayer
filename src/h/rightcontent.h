//
// Created by edward on 2026/4/10.
//

#ifndef ECHONOTE_RIGHTCONTENT_H
#define ECHONOTE_RIGHTCONTENT_H

#include <QStackedWidget>
#include "songwidget.h"
#include "albumwidget.h"

class RightContent : public QStackedWidget {
    Q_OBJECT

public:
    explicit RightContent(QWidget *parent = nullptr);

    ~RightContent() override;
public slots:
    void showPage(const QString &key);

private:
};


#endif //ECHONOTE_RIGHTCONTENT_H
