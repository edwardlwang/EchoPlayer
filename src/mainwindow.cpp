//
// Created by edward on 2026/3/25.
//

#include "mainwindow.h"
#include "playercontrolbar.h"
#include "leftnavigationbar.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    qDebug() << "MainWindow构造完成";

    // 设置主窗口属性
    this->resize(960, 540);
    this->setWindowTitle("EchoNote");
    this->setWindowIcon(QIcon("../res/icons/EchoNote_icon.png"));

    // 为主窗口添加中心控件，所有子控件都添加在中心控件里
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // 为中心控件添加垂直布局：上方内容区，下方底部状态栏
    centralWidgetLayout = new QVBoxLayout(centralWidget);
    centralWidgetLayout->setContentsMargins(0, 0, 0, 0);
    centralWidgetLayout->setSpacing(0);

    // 上方内容区容器
    contentWidget = new QWidget(centralWidget);
    contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // 左侧导航栏
    const auto leftNavigationBar = new LeftNavigationBar(contentWidget);
    contentLayout->addWidget(leftNavigationBar);

    // 右侧内容区域
    rightContent = new QWidget(contentWidget);
    contentLayout->addWidget(rightContent, 1);
    rightContent->setObjectName("rightContent");

    rightContentLayout = new QHBoxLayout(rightContent);
    rightContentLayout->setContentsMargins(0, 0, 0, 0);
    rightContentLayout->setSpacing(0);

    // 上方内容区占据剩余空间
    centralWidgetLayout->addWidget(contentWidget, 1);

    // 底部状态栏固定在窗口底部
    auto bottomStatusBar = new PlayerControlBar(centralWidget);
    centralWidgetLayout->addWidget(bottomStatusBar);
}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow析构完成！";
}





