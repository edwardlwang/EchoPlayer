//
// Created by edward on 2026/3/25.
//

#ifndef MUSICPLAYER_MAINWINDOW_H
#define MUSICPLAYER_MAINWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include "navigationbar.h"
#include "playercontrolbar.h"
#include "contentarea.h"

class MainWindow : public QMainWindow {
    // Q_OBJECT 是 Qt 框架的核心灵魂宏，它是 Qt 实现信号与槽（Signals & Slots）、元对象系统（Meta-Object System）等核心特性的必要前提。
    // 只要你的类要用到 Qt 的信号槽、属性系统、动态类型转换等功能，就必须在类定义的 private 区第一行写上 Q_OBJECT。
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

private:
    QWidget *m_central_widget; // 主窗口中心容器（setCentralWidget）
    QVBoxLayout *m_central_widget_layout; // 中心容器的垂直布局：上内容区、下控制栏
    QWidget *m_content_widget; // 上方内容区容器
    QHBoxLayout *m_content_layout; // 内容区水平布局：左导航 + 右侧内容

    NavigationBar *m_navigation_bar; // 左侧导航栏
    ContentArea *m_content; // 右侧主内容容器
    PlayerControlBar *m_player_control_bar; // 底部播放器控制栏
};

#endif //MUSICPLAYER_MAINWINDOW_H
