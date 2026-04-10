//
// Created by edward on 2026/4/10.
//

#include "../h/rightcontent.h"

RightContent::RightContent(QWidget *parent) : QStackedWidget(parent) {
    qDebug() << "RightContent构造完成";
    this->setObjectName("rightContent");
    this->setAttribute(Qt::WA_StyledBackground, true);
}

RightContent::~RightContent() {
    qDebug() << "RightContent析构完成";
}
