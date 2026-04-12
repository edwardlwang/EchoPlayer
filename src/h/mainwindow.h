//
// Created by edward on 2026/3/25.
//

#ifndef MUSICPLAYER_MAINWINDOW_H
#define MUSICPLAYER_MAINWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "leftnavigationbar.h"
#include "playercontrolbar.h"
#include "rightcontent.h"

class MainWindow : public QMainWindow {
    // Q_OBJECT 是 Qt 框架的核心灵魂宏，它是 Qt 实现信号与槽（Signals & Slots）、元对象系统（Meta-Object System）等核心特性的必要前提。
    // 只要你的类要用到 Qt 的信号槽、属性系统、动态类型转换等功能，就必须在类定义的 private 区第一行写上 Q_OBJECT。
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

private:
    QWidget *centralWidget; // 主窗口中心容器（setCentralWidget）
    QVBoxLayout *centralWidgetLayout; // 中心容器的垂直布局：上内容区、下控制栏
    QWidget *contentWidget; // 上方内容区容器
    QHBoxLayout *contentLayout; // 内容区水平布局：左导航 + 右侧内容

    LeftNavigationBar *leftNavigationBar; // 左侧导航栏
    RightContent *rightContent; // 右侧主内容容器
    PlayerControlBar *playerControlBar; // 底部播放器控制栏
};

#endif //MUSICPLAYER_MAINWINDOW_H
