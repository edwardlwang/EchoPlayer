//
// Created by edward on 2026/3/25.
//

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    this->resize(960, 540);
    this->setWindowTitle("EchoNote");
    this->setWindowIcon(QIcon("../res/icons/EchoNote_icon.png"));
}

MainWindow::~MainWindow()=default;





