#include "weatherwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString fontPath = ":/res/AiDianGanFengXingKai-2.ttf";  // 自定义字体文件的路径
    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            QFont customFont(fontFamilies.at(0), 15);  // 创建自定义字体对象
            a.setFont(customFont);
        } else {
            qDebug() << "Failed to load the font from path:" << fontPath;
        }
    }
    w.show();
    return a.exec();
}
