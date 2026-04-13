//
// Created by edward on 2026/4/10.
//

#ifndef ECHONOTE_CONTENTAREA_H
#define ECHONOTE_CONTENTAREA_H

#include <QStackedWidget>
#include <QHash>
#include "songpage.h"
#include "albumpage.h"
#include "artistpage.h"
#include "playlistpage.h"
#include "settingpage.h"

class ContentArea : public QStackedWidget {
    Q_OBJECT

public:
    explicit ContentArea(QWidget *parent = nullptr);

    ~ContentArea() override;

public slots:
    void showPage(const QString &key);

private:
    QHash<QString, int> m_page_index_map;
};


#endif //ECHONOTE_CONTENTAREA_H
