//
// Created by edward on 2026/4/13.
//

#ifndef ECHONOTE_SETTINGPAGE_H
#define ECHONOTE_SETTINGPAGE_H
#include <QWidget>

class SettingPage : public QWidget {
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);

    ~SettingPage() override;
};


#endif //ECHONOTE_SETTINGPAGE_H
