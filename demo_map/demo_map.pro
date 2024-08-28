TEMPLATE = lib
TARGET = DemoMap
QT += core gui webenginewidgets webchannel widgets

#CONFIG += c++11

DEFINES += DEMOMAP_LIBRARY

DESTDIR = ../bin

LIBS += -L/usr/lib -lcurl

win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错

}

unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    getlnglat.cpp \
    mapwidget.cpp \
    mychannel.cpp \
    main.cpp

HEADERS += \
    CustomStyle.h \
    mapwidget.h \
    mychannel.h

FORMS += \
    mapwidget.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    html.qrc

#DESTDIR = $$PWD
