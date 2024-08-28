#include <QApplication>
#include "alarmmainwindow.h"
#include "fileio.h"
#include "alarmmainwindow.h"
#include "myclockwidgetfinal.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("CLock");
    a.setOrganizationName("CLock");
    alarmMainWindow w;
    if(FileIO::LoadWindowShow() || !QSystemTrayIcon::isSystemTrayAvailable())
    {
        w.show();
    }else{
        w.show();
        w.hide();
    }
    return a.exec();
}
