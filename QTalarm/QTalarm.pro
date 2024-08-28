QT       += core gui multimedia widgets\

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTalarm
TEMPLATE = lib
DESTDIR = ../bin
DEFINES += QTALARM_LIBRARY
win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错

}

unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES +=\
    alarmmainwindow.cpp \
    bastardsnooze.cpp \
    myclockwidget.cpp \
    myclockwidgetfinal.cpp \
    poemfetcher.cpp \
    schedulemodel.cpp \
    timer.cpp \
    alarm.cpp \
    fileio.cpp \
    schedules.cpp \
    aboutdialog.cpp \
    snooze.cpp \
    settingdialog.cpp \
    waterprogressbar.cpp

HEADERS  += \
    alarmmainwindow.h \
    bastardsnooze.h \
    myclockwidget.h \
    myclockwidgetfinal.h \
    poemfetcher.h \
    schedulemodel.h \
    timer.h \
    alarm.h \
    fileio.h \
    schedules.h \
    aboutdialog.h \
    snooze.h \
    settingdialog.h \
    ui_aboutdialog.h \
    waterprogressbar.h

FORMS    += \
    about.ui \
    alarmmainwindow.ui \
    bastardsnooze.ui \
    snooze.ui \
    settingdialog.ui

RESOURCES += \
    Icons.qrc
CONFIG += precompile_header
