/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/

#include "browser.h"
#include "browserwindow.h"


Browser::Browser()//set download action
{
    m_downloadManagerWidget.setAttribute(Qt::WA_QuitOnClose, false);

    QObject::connect(
        QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested,
        &m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);
}

BrowserWindow *Browser::createWindow(bool offTheRecord)//private browsing mode
{
    if (offTheRecord && !m_otrProfile) {//m_otrProfile is a private mode instance
        m_otrProfile.reset(new QWebEngineProfile);
        QObject::connect(
            m_otrProfile.get(), &QWebEngineProfile::downloadRequested,
            &m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);
    }
    auto profile = offTheRecord ? m_otrProfile.get() : QWebEngineProfile::defaultProfile();
    auto mainWindow = new BrowserWindow(this, profile, false);
    m_windows.append(mainWindow);
    QObject::connect(mainWindow, &QObject::destroyed, [this, mainWindow]() {
        m_windows.removeOne(mainWindow);
    });
    mainWindow->show();
    return mainWindow;
}

// A member function of the Browser class to create a new browser window, potentially dedicated for developer tools
BrowserWindow *Browser::createDevToolsWindow()
{
    // Retrieve the default web engine profile, which contains browser settings like cookies, cache, etc.
    auto profile = QWebEngineProfile::defaultProfile();

    // Create a new BrowserWindow instance, passing in a pointer to the current Browser instance (this),
    // the default profile, and a boolean value indicating if this window is special (potentially for developer tools).
    // Note: The true value here might be a convention indicating this window differs from regular browsing windows,
    // but the specific meaning depends on the implementation of BrowserWindow.
    auto mainWindow = new BrowserWindow(this, profile, true);

    // Append the newly created window to the Browser class's window list for management.
    // m_windows is likely a member variable of the Browser class, storing all BrowserWindow instances created by this Browser instance.
    m_windows.append(mainWindow);

    // When the mainWindow is destroyed, remove it from the m_windows list.
    // This is achieved using QObject's destroyed signal and a lambda expression to avoid dangling pointers and ensure proper memory management.
    QObject::connect(mainWindow, &QObject::destroyed, [this, mainWindow]() {
        m_windows.removeOne(mainWindow);
    });

    // Show the newly created browser window.
    // This step is optional, depending on whether you want the window to be visible immediately after creation.
    // In some cases, you might want to configure the window further before showing it.
    mainWindow->show();

    // Return a pointer to the newly created browser window,
    // allowing the caller to access and manipulate this new window.
    return mainWindow;
}
