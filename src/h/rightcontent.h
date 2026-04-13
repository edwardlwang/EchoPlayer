//
// Created by edward on 2026/4/10.
//

#ifndef ECHONOTE_RIGHTCONTENT_H
#define ECHONOTE_RIGHTCONTENT_H

#include <QStackedWidget>
#include <QHash>
#include "songpage.h"
#include "albumpage.h"
#include "artistpage.h"
#include "playlistpage.h"
#include "settingpage.h"

class RightContent : public QStackedWidget {
    Q_OBJECT

public:
    explicit RightContent(QWidget *parent = nullptr);

    ~RightContent() override;

public slots:
    void showPage(const QString &key);

private:
    QHash<QString, int> pageIndexMap;
};


#endif //ECHONOTE_RIGHTCONTENT_H
