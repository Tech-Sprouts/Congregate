/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QWebEnginePage>
#include <QListWidget>
#include <QPushButton>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QProgressBar;
QT_END_NAMESPACE

class Browser;
class TabWidget;
class WebView;

class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow(Browser *browser, QWebEngineProfile *profile, bool forDevTools = false);
    QSize sizeHint() const override;
    TabWidget *tabWidget() const;
    WebView *currentTab() const;
    Browser *browser() { return m_browser; }
    void ModifyIcon();

protected:
    void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *key) override;

private slots:
    void handleNewWindowTriggered();
    void handleNewIncognitoWindowTriggered();
    void handleFileOpenTriggered();
    void handleFindActionTriggered();
    void handleShowWindowTriggered();
    void handleWebViewLoadProgress(int);
    void handleWebViewTitleChanged(const QString &title);
    void handleWebActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled);
    void handleDevToolsRequested(QWebEnginePage *source);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    void handleFindTextFinished(const QWebEngineFindTextResult &result);
#endif
    void about();

private:
    QMenu *createFileMenu(TabWidget *tabWidget);
    QMenu *createEditMenu();
    QMenu *createViewMenu(QToolBar *toolBar);
    QMenu *createWindowMenu(TabWidget *tabWidget);
    QMenu *createHelpMenu();
    QMenu *createBookMarkMenu();
    QPushButton *closeMarkListButton;
    QToolBar *createToolBar();
private slots:
    void showBookmarks();
    QStringList getAllBookmarks();
    void onBookmarkClicked(QListWidgetItem *item);

private:
    Browser *m_browser;
    QWebEngineProfile *m_profile;
    TabWidget *m_tabWidget;

    //progress bar
    QProgressBar *m_progressBar;

    //some action
    QAction *m_historyBackAction;
    QAction *m_historyForwardAction;
    QAction *m_stopAction;
    QAction *m_reloadAction;
    QAction *m_stopReloadAction;

    //about bookmark
    QAction *m_starAction;
    int isstar=0;
    QSqlDatabase db;
    void modifyicon();

    //url line
    QLineEdit *m_urlLineEdit;
    QAction *m_favAction;
    QString m_lastSearch;
    QToolBar *toolBar;
};

#endif // BROWSERWINDOW_H
