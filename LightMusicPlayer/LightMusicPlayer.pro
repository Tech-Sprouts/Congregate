QT       += core gui sql
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LightMusicPlayer
TEMPLATE = lib
DESTDIR = ../bin
DEFINES += LIGHTMUSICPLAYER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错

}

unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    target.path = /usr/lib
    INSTALLS += target
}
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    CustomWidget.cpp \
    LyricWidget.cpp \
    MusicMainwindow.cpp \
    main.cpp \
    Music.cpp \
    MusicListDialog.cpp \
    rollinglabel.cpp \
    rotatablelabel.cpp

HEADERS += \
    CustomWidget.h \
    LyricWidget.h \
    Music.h \
    MusicListWidget.h \
    MusicMainwindow.h \
    myQSS.h \
    MusicListDialog.h \
    rollinglabel.h \
    rotatablelabel.h

FORMS += \
    LyricWidget.ui \
    MusicListDialog.ui \
    MusicMainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imagefile.qrc


RC_ICONS = LightMusicPlayer.ico
