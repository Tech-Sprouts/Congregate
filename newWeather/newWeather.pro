QT += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = newWeather
TEMPLATE = lib
DESTDIR = ../bin
DEFINES += NEWWEATHER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错

}

unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    target.path = /usr/lib
    INSTALLS += target
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 添加 curl 库路径和链接选项
LIBS += -lcurl

SOURCES += \
    basewea.cpp \
    locationfetcher.cpp \
    citywithtemp.cpp \
    weatherwindow.cpp \
    worldlistwea.cpp

HEADERS += \
    basewea.h \
    locationfetcher.h \
    weatherData.h \
    weatherTool.h \
    citywithtemp.h \
    weatherwindow.h \
    worldlistwea.h

FORMS += \
    basewea.ui \
    citywithtemp.ui \
    weatherwindow.ui \
    worldlistwea.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    citycode-2019-08-23.json \
    res/Search2.png
