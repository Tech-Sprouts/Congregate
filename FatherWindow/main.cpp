#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置全局样式表
    app.setStyleSheet("QMainWindow { background-color: #f0f0f0; }");

    MainWindow window;
    window.show();

    return app.exec();
}
