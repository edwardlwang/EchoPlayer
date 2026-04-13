//
// Created by edward on 2026/3/25.
//

#include "../h/mainwindow.h"
#include "../h/contentarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    qDebug() << "MainWindow构造完成";

    // 设置主窗口属性
    this->resize(960, 540);
    this->setWindowTitle("EchoNote");
    this->setWindowIcon(QIcon(":/icons/music.svg"));

    // 为主窗口添加中心控件，所有子控件都添加在中心控件里
    m_central_widget = new QWidget(this);
    this->setCentralWidget(m_central_widget);

    // 为中心控件添加垂直布局：上方内容区，下方底部状态栏
    m_central_widget_layout = new QVBoxLayout(m_central_widget);
    m_central_widget_layout->setContentsMargins(0, 0, 0, 0);
    m_central_widget_layout->setSpacing(0);

    // 上方内容区容器
    m_content_widget = new QWidget(m_central_widget);
    m_content_layout = new QHBoxLayout(m_content_widget);
    m_content_layout->setContentsMargins(0, 0, 0, 0);
    m_content_layout->setSpacing(0);

    // 左侧导航栏
    m_navigation_bar = new NavigationBar(m_content_widget);
    m_content_layout->addWidget(m_navigation_bar);

    // 右侧内容区域
    m_content = new ContentArea(m_content_widget);
    m_content_layout->addWidget(m_content, 1);

    connect(m_navigation_bar, &NavigationBar::navigationButtonClicked,
        m_content, &ContentArea::showPage);

    // 上方内容区占据剩余空间
    m_central_widget_layout->addWidget(m_content_widget, 1);

    // 底部状态栏固定在窗口底部
    m_player_control_bar = new PlayerControlBar(m_central_widget);
    m_central_widget_layout->addWidget(m_player_control_bar);


}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow析构完成";
}





