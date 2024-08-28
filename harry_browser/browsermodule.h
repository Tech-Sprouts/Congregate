#ifndef BROWSERMODULE_H
#define BROWSERMODULE_H

#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include <QApplication>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QScreen>
#include <QTranslator>

class browsermodule {
public:
    browsermodule(){};
    void showBrowser(const QUrl &url = QUrl(QStringLiteral("https://www.baidu.com")));

private:
    Browser browser;
    BrowserWindow *window;
};

#endif // BROWSERMODULE_H
