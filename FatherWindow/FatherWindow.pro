QT += core gui widgets webenginewidgets webenginecore network webchannel \
    printsupport sql webengine webchannel \
    multimedia charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = FatherWindow

TEMPLATE = app

DESTDIR = ../bin

win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    LIBS += ../bin/HarryBrowser.lib #包含项目需要用到的共享库
    LIBS += ../bin/DemoMap.lib
    LIBS += ../bin/QTalarm.lib
    LIBS += ../bin/newWeather.lib
    LIBS += ../bin/LightMusicPlayer.lib
}

unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    LIBS += ../bin/libHarryBrowser.so #包含项目需要用到的共享库，注意文件名多了关键字"lib"，扩展名是.so
    LIBS += ../bin/libDemoMap.so
    LIBS += ../bin/libQTalarm.so
    LIBS += ../bin/libnewWeather.so
    LIBS += ../bin/libLightMusicPlayer.so
}

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target




#QT += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = MainSurface

#TEMPLATE = app

#DESTDIR = ../bin

#win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
#    LIBS += ../bin/Print.lib #包含项目需要用到的共享库
#}

#unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
#    LIBS += ../bin/libPrint.so #包含项目需要用到的共享库，注意文件名多了关键字"lib"，扩展名是.so
#}


#CONFIG += c++11

#SOURCES += \
#    main.cpp \
#    widget.cpp

#HEADERS += \
#    widget.h

#FORMS += \
#    widget.ui
