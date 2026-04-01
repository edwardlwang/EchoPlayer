//
// Created by edward on 2026/3/25.
//

#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    std::cout << "MainWindow的构造函数\n";
    this->resize(960, 540);
    this->setWindowTitle("EchoNote");
    this->setWindowIcon(QIcon("../res/icons/EchoNote_icon.png"));

    // 左侧工具栏
    leftToolBar = new QToolBar(this);
    this->addToolBar(Qt::LeftToolBarArea, leftToolBar);
    leftToolBar->setObjectName("leftToolBar");
    leftToolBar->setMovable(false);
    leftToolBar->setAllowedAreas(Qt::LeftToolBarArea);
}

MainWindow::~MainWindow() {
    std::cout << "MainWindow的析构函数\n";
}





