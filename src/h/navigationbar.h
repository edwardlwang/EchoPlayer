//
// Created by edward on 2026/4/4.
//

#ifndef ECHONOTE_NAVIGATIONBAR_H
#define ECHONOTE_NAVIGATIONBAR_H

#include <vector>
#include <QPushButton>
#include <QVBoxLayout>
#include <QButtonGroup>

class NavigationBar : public QWidget {
    Q_OBJECT

public:
    NavigationBar(QWidget *parent = nullptr);

    ~NavigationBar() override;

signals:
    void navigationButtonClicked(const QString &key);

private:
    QVBoxLayout *m_navigation_bar_layout;
    std::vector<QPushButton *> m_navigation_bar_buttons;
    QButtonGroup *m_navigation_bar_button_group;

    struct BtnInfo {
        QString key;
        QString text;
        QString iconPath;
    };
};


#endif //ECHONOTE_NAVIGATIONBAR_H
