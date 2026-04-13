//
// Created by edward on 2026/3/25.
//

#include "../h/mainwindow.h"
#include "../h/rightcontent.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    qDebug() << "MainWindow构造完成";

    // 设置主窗口属性
    this->resize(960, 540);
    this->setWindowTitle("EchoNote");
    this->setWindowIcon(QIcon(":/icons/music.svg"));

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
    leftNavigationBar = new LeftNavigationBar(contentWidget);
    contentLayout->addWidget(leftNavigationBar);

    // 右侧内容区域
    rightContent = new RightContent(contentWidget);
    contentLayout->addWidget(rightContent, 1);

    connect(leftNavigationBar, &LeftNavigationBar::navigationButtonClicked,
        rightContent, &RightContent::showPage);

    // 上方内容区占据剩余空间
    centralWidgetLayout->addWidget(contentWidget, 1);

    // 底部状态栏固定在窗口底部
    playerControlBar = new PlayerControlBar(centralWidget);
    centralWidgetLayout->addWidget(playerControlBar);


}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow析构完成";
}





