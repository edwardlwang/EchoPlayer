//
// Created by edward on 2026/3/25.
//

#ifndef MUSICPLAYER_MAINWINDOW_H
#define MUSICPLAYER_MAINWINDOW_H


#include "lefttoolbar.h"
#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <vector>

class MainWindow : public QMainWindow {
    // Q_OBJECT 是 Qt 框架的核心灵魂宏，它是 Qt 实现信号与槽（Signals & Slots）、元对象系统（Meta-Object System）等核心特性的必要前提。
    // 只要你的类要用到 Qt 的信号槽、属性系统、动态类型转换等功能，就必须在类定义的 private 区第一行写上 Q_OBJECT。
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

private:
    LeftToolBar *leftToolBar;
};

#endif //MUSICPLAYER_MAINWINDOW_H
