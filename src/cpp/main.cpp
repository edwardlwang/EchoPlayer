#include "../h/mainwindow.h"
#include <QApplication>
#include <QFile>

// main程序入口 argc为命令行变量的数量 argv为命令行变量的数组
int main(int argc, char *argv[]) {
    // 应用程序对象，在Qt项目中，有且只有一个
    QApplication a(argc, argv);

    if (QFile f(":/styles/app.qss"); f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(QString::fromUtf8(f.readAll()));
    } else {
        qWarning() << "Failed to load stylesheet:" << f.errorString();
    }

    MainWindow main_window;
    main_window.show();

    // 让应用程序对象进入消息循环，使代码阻塞到这一行，类似while(true)
    return QApplication::exec();
}
