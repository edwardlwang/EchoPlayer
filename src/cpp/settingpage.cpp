//
// Created by edward on 2026/4/13.
//

#include "../h/settingpage.h"

SettingPage::SettingPage(QWidget *parent) : QWidget(parent) {
    qDebug() << "SettingPage构造完成";
    this->setObjectName("settingpage");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

SettingPage::~SettingPage() {
    qDebug() << "SettingPage析构完成";
}
