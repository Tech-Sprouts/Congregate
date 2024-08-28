#include "mapwidget.h"
#include <QApplication>
#include "CustomStyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(new CustomStyle());

    MapWidget w;
    w.show();
    return a.exec();
}
