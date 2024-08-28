TEMPLATE = lib
TARGET = HarryBrowser
QT += core gui webenginewidgets webenginecore network webchannel printsupport sql widgets webengine

DEFINES += HARRYBROWSER_LIBRARY

DESTDIR = ../bin

win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错

}

unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    target.path = /usr/lib
    INSTALLS += target
}


HEADERS += \
    HarryBrowser_global.h \
    browser.h \
    browsermodule.h \
    browserwindow.h \
    downloadmanagerwidget.h \
    downloadwidget.h \
    tabwidget.h \
    webpage.h \
    webpopupwindow.h \
    webview.h \
    printhandler.h

SOURCES += \
    browser.cpp \
    browsermodule.cpp \
    browserwindow.cpp \
    downloadmanagerwidget.cpp \
    downloadwidget.cpp \
    tabwidget.cpp \
    webpage.cpp \
    webpopupwindow.cpp \
    webview.cpp \
    printhandler.cpp

FORMS += \
    certificateerrordialog.ui \
    passworddialog.ui \
    downloadmanagerwidget.ui \
    downloadwidget.ui

RESOURCES += \
    res/myOpenBrowser.qrc \
    i18n/language.qrc

TRANSLATIONS += \
    i18n/en.ts \
    i18n/zh_CN.ts

RC_ICONS = res/AppLogo.ico

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/myOpenBrowser
#INSTALLS += target

#DESTDIR = ./dist

##windows属性信息资源配置文件(文件编码需为ANSI)
#RC_FILE = ./res/win_res.rc

#DISTFILES +=
