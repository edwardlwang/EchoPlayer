//
// Created by edward on 2026/3/25.
//

#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    std::cout << "MainWindow的构造函数\n";

    // 设置主窗口属性
    this->resize(960, 540);
    this->setWindowTitle("EchoNote");
    this->setWindowIcon(QIcon("../res/icons/EchoNote_icon.png"));

    // 为主窗口添加中心控件，所有子控件都添加在中心控件里
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // 为中心控件添加水平布局
    centralWidgetLayout = new QHBoxLayout(centralWidget);
    centralWidgetLayout->setContentsMargins(0, 0, 0, 0);
    centralWidgetLayout->setSpacing(0);

    // 左侧导航栏
    leftNavigationBar = new QWidget(centralWidget);

    leftNavigationBar->setObjectName("leftNavigationBar");
    leftNavigationBar->setFixedWidth(108);

    leftNavigationBarLayout = new QVBoxLayout(leftNavigationBar);
    leftNavigationBarLayout->setContentsMargins(0, 0, 0, 0);
    leftNavigationBarLayout->setSpacing(0);
    rightContent = new QWidget(centralWidget);
    rightContent->setObjectName("rightContent");

    rightContentLayout = new QHBoxLayout(rightContent);
    rightContentLayout->setContentsMargins(0, 0, 0, 0);
    rightContentLayout->setSpacing(0);
}

MainWindow::~MainWindow() {
    std::cout << "MainWindow的析构函数\n";
}





