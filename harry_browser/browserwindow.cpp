/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/

//bug:
//切换界面，收藏无法刷新
//新建标签页后无法显示收藏

#include "browser.h"
#include "browserwindow.h"
#include "downloadmanagerwidget.h"
#include "tabwidget.h"
#include "webview.h"
#include "printhandler.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QScreen>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QWebEngineFindTextResult>
#endif
#include <QWebEngineProfile>

BrowserWindow::BrowserWindow(Browser *browser, QWebEngineProfile *profile, bool forDevTools)
    : m_browser(browser)
    , m_profile(profile)
    , m_tabWidget(new TabWidget(profile, this))
    , m_progressBar(nullptr)
    , m_historyBackAction(nullptr)
    , m_historyForwardAction(nullptr)
    , m_stopAction(nullptr)
    , m_reloadAction(nullptr)
    , m_stopReloadAction(nullptr)
    , m_urlLineEdit(nullptr)
    , m_favAction(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose, true);//when you close the window, delete it.
    setFocusPolicy(Qt::ClickFocus);//how you can change your focus

    if (!forDevTools) {
        m_progressBar = new QProgressBar(this);

        //just add some useful components
        QToolBar *toolbar = createToolBar();
        toolBar = toolbar;
        addToolBar(toolbar);
        menuBar()->addMenu(createFileMenu(m_tabWidget));
        menuBar()->addMenu(createEditMenu());
        menuBar()->addMenu(createViewMenu(toolbar));
        menuBar()->addMenu(createWindowMenu(m_tabWidget));
//        menuBar()->addMenu(createHelpMenu());
        menuBar()->addMenu(createBookMarkMenu());
    }

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    //set margin? Tight
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    if (!forDevTools) {
        addToolBarBreak();

        m_progressBar->setMaximumHeight(1);
        m_progressBar->setTextVisible(false);
        //here is a stylesheet
        m_progressBar->setStyleSheet(QStringLiteral("QProgressBar {border: 0px} QProgressBar::chunk {background-color: #da4453}"));

        layout->addWidget(m_progressBar);
    }

    layout->addWidget(m_tabWidget);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    //change title when title change
    connect(m_tabWidget, &TabWidget::titleChanged, this, &BrowserWindow::handleWebViewTitleChanged);

    if (!forDevTools) {
        connect(m_tabWidget, &TabWidget::linkHovered, [this](const QString& url) {//mouse hover, show message
            statusBar()->showMessage(url);
        });
        connect(m_tabWidget, &TabWidget::loadProgress, this, &BrowserWindow::handleWebViewLoadProgress);//show progress
        connect(m_tabWidget, &TabWidget::webActionEnabledChanged, this, &BrowserWindow::handleWebActionEnabledChanged);//some action like ctrl+C
        connect(m_tabWidget, &TabWidget::urlChanged, [this](const QUrl &url) {//when url in website change, change url text in url line
            m_urlLineEdit->setText(url.toDisplayString());
            //see if url in db
            modifyicon();
        });
        /*==========FAVOURITE=============*/
        connect(m_tabWidget, &TabWidget::favIconChanged, m_favAction, &QAction::setIcon);//fav? where can I get access into it
        connect(m_tabWidget, &TabWidget::devToolsRequested, this, &BrowserWindow::handleDevToolsRequested);
        connect(m_urlLineEdit, &QLineEdit::returnPressed, [this]() {//press "enter", send url
            m_tabWidget->setUrl(QUrl::fromUserInput(m_urlLineEdit->text()));
            //see if url in db
            modifyicon();
        });

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        connect(m_tabWidget, &TabWidget::findTextFinished, this, &BrowserWindow::handleFindTextFinished);
#endif

        //when you press ctrl+L, you focus on the url edit line
        QAction *focusUrlLineEditAction = new QAction(this);
        addAction(focusUrlLineEditAction);
        focusUrlLineEditAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));//set shortcut key
        connect(focusUrlLineEditAction, &QAction::triggered, this, [this] () {
            m_urlLineEdit->setFocus(Qt::ShortcutFocusReason);
        });
    }

    handleWebViewTitleChanged(QString());//set the url to nothing
    m_tabWidget->createTab();//create a tabwidget
    //manually set icon
    modifyicon();
}

QSize BrowserWindow::sizeHint() const
{
    //get main window geometry
    QRect desktopRect = QApplication::primaryScreen()->geometry();
    QSize size = desktopRect.size() * qreal(0.9);//set to 90%, why?
    return size;
}

/**
 * 文件菜单
 * @brief BrowserWindow::createFileMenu
 * @param tabWidget
 * @return
 */
QMenu *BrowserWindow::createFileMenu(TabWidget *tabWidget)
{
    QMenu *fileMenu = new QMenu(tr("&File"));
    //add action, new window
    fileMenu->addAction(tr("&New Window"), this, &BrowserWindow::handleNewWindowTriggered, QKeySequence::New);//default to be ctrl+N
    fileMenu->addAction(tr("New &Incognito Window"), this, &BrowserWindow::handleNewIncognitoWindowTriggered,QKeySequence(tr("Ctrl+Shift+N")));

    //add action, new tab
    QAction *newTabAction = new QAction(tr("New &Tab"), this);
    newTabAction->setShortcuts(QKeySequence::AddTab);
    connect(newTabAction, &QAction::triggered, this, [this]() {//connect tab to slot
        m_tabWidget->createTab();
        m_urlLineEdit->setFocus();
    });
    fileMenu->addAction(newTabAction);

    fileMenu->addAction(tr("&Open File..."), this, &BrowserWindow::handleFileOpenTriggered, QKeySequence::Open);
    fileMenu->addSeparator();

    QAction *closeTabAction = new QAction(tr("&Close Tab"), this);
    closeTabAction->setShortcuts(QKeySequence::Close);
    connect(closeTabAction, &QAction::triggered, [tabWidget]() {
        tabWidget->closeTab(tabWidget->currentIndex());
    });
    fileMenu->addAction(closeTabAction);

    QAction *printTabPageAction = new QAction(tr("&Print"),this);
    printTabPageAction->setShortcut(QKeySequence::Print);
    connect(printTabPageAction, &QAction::triggered, [tabWidget]() {
        tabWidget->printPreview();
    });
//    fileMenu->addAction(printTabPageAction);

//    QAction *printPdfTabPageAction = new QAction(tr("&Print to PDF"),this);
//    printPdfTabPageAction->setShortcut(QKeySequence(tr("Ctrl+Shift+P")));
//    connect(printPdfTabPageAction, &QAction::triggered, [tabWidget]() {
//        tabWidget->printToPDF();
//    });
//    fileMenu->addAction(printPdfTabPageAction);

    QAction *closeAction = new QAction(tr("&Quit"),this);
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(closeAction);

    connect(fileMenu, &QMenu::aboutToShow, [this, closeAction]() {
        if (m_browser->windows().count() == 1)
            closeAction->setText(tr("&Quit"));
        else
            closeAction->setText(tr("&Close Window"));
    });
    return fileMenu;
}
/**
 * 编辑菜单
 * @brief BrowserWindow::createEditMenu
 * @return
 */
QMenu *BrowserWindow::createEditMenu()
{
    QMenu *editMenu = new QMenu(tr("&Edit"));
    QAction *findAction = editMenu->addAction(tr("&Find"));
    findAction->setShortcuts(QKeySequence::Find);
    connect(findAction, &QAction::triggered, this, &BrowserWindow::handleFindActionTriggered);

    QAction *findNextAction = editMenu->addAction(tr("Find &Next"));
    findNextAction->setShortcut(QKeySequence::FindNext);
    connect(findNextAction, &QAction::triggered, [this]() {
        if (!currentTab() || m_lastSearch.isEmpty())
            return;
        currentTab()->findText(m_lastSearch);
    });

    QAction *findPreviousAction = editMenu->addAction(tr("Find &Previous"));
    findPreviousAction->setShortcut(QKeySequence::FindPrevious);
    connect(findPreviousAction, &QAction::triggered, [this]() {
        if (!currentTab() || m_lastSearch.isEmpty())
            return;
        currentTab()->findText(m_lastSearch, QWebEnginePage::FindBackward);
    });

    return editMenu;
}

/**
 * 查看菜单
 * @brief BrowserWindow::createViewMenu
 * @param toolbar
 * @return
 */
QMenu *BrowserWindow::createViewMenu(QToolBar *toolbar)
{
    QMenu *viewMenu = new QMenu(tr("&View"));
    m_stopAction = viewMenu->addAction(tr("&Stop"));
    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Period));
    shortcuts.append(Qt::Key_Escape);
    m_stopAction->setShortcuts(shortcuts);
    connect(m_stopAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Stop);
    });

    m_reloadAction = viewMenu->addAction(tr("Reload Page"));
    m_reloadAction->setShortcuts(QKeySequence::Refresh);
    connect(m_reloadAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Reload);
    });

    QAction *zoomIn = viewMenu->addAction(tr("Zoom &In"));
    zoomIn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus));
    connect(zoomIn, &QAction::triggered, [this]() {
        if (currentTab())
            currentTab()->setZoomFactor(currentTab()->zoomFactor() + 0.1);
    });

    QAction *zoomOut = viewMenu->addAction(tr("Zoom &Out"));
    zoomOut->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
    connect(zoomOut, &QAction::triggered, [this]() {
        if (currentTab())
            currentTab()->setZoomFactor(currentTab()->zoomFactor() - 0.1);
    });

    QAction *resetZoom = viewMenu->addAction(tr("Reset &Zoom"));
    resetZoom->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(resetZoom, &QAction::triggered, [this]() {
        if (currentTab())
            currentTab()->setZoomFactor(1.0);
    });



    viewMenu->addSeparator();
    QAction *viewToolbarAction = new QAction(tr("Hide Toolbar"),this);
    viewToolbarAction->setShortcut(tr("Ctrl+|"));
    connect(viewToolbarAction, &QAction::triggered, [toolbar,viewToolbarAction]() {
        if (toolbar->isVisible()) {
            viewToolbarAction->setText(tr("Show Toolbar"));
            toolbar->close();
        } else {
            viewToolbarAction->setText(tr("Hide Toolbar"));
            toolbar->show();
        }
    });
    viewMenu->addAction(viewToolbarAction);

    QAction *viewStatusbarAction = new QAction(tr("Hide Status Bar"), this);
    viewStatusbarAction->setShortcut(tr("Ctrl+/"));
    connect(viewStatusbarAction, &QAction::triggered, [this, viewStatusbarAction]() {
        if (statusBar()->isVisible()) {
            viewStatusbarAction->setText(tr("Show Status Bar"));
            statusBar()->close();
        } else {
            viewStatusbarAction->setText(tr("Hide Status Bar"));
            statusBar()->show();
        }
    });
    viewMenu->addAction(viewStatusbarAction);
    return viewMenu;
}
/**
 * 窗口菜单
 * @brief BrowserWindow::createWindowMenu
 * @param tabWidget
 * @return
 */
QMenu *BrowserWindow::createWindowMenu(TabWidget *tabWidget)
{
    QMenu *menu = new QMenu(tr("&Window"));

    QAction *nextTabAction = new QAction(tr("Show Next Tab"), this);
    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceRight));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageDown));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketRight));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Less));
    nextTabAction->setShortcuts(shortcuts);
    connect(nextTabAction, &QAction::triggered, tabWidget, &TabWidget::nextTab);

    QAction *previousTabAction = new QAction(tr("Show Previous Tab"), this);
    shortcuts.clear();
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceLeft));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageUp));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketLeft));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Greater));
    previousTabAction->setShortcuts(shortcuts);
    connect(previousTabAction, &QAction::triggered, tabWidget, &TabWidget::previousTab);

    connect(menu, &QMenu::aboutToShow, [this, menu, nextTabAction, previousTabAction]() {
        menu->clear();
        menu->addAction(nextTabAction);
        menu->addAction(previousTabAction);
        menu->addSeparator();

        QVector<BrowserWindow*> windows = m_browser->windows();
        int index(-1);
        for (auto window : windows) {
            QAction *action = menu->addAction(window->windowTitle(), this, &BrowserWindow::handleShowWindowTriggered);
            action->setData(++index);
            action->setCheckable(true);
            if (window == this)
                action->setChecked(true);
        }
    });
    return menu;
}
/**
 * 帮助菜单
 * @brief BrowserWindow::createHelpMenu
 * @return
 */
QMenu *BrowserWindow::createHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    aboutAction->setShortcuts(QKeySequence::HelpContents);
    connect(aboutAction, &QAction::triggered, this, &BrowserWindow::about);
    return helpMenu;
}
/**
 * 收藏菜单
*/
QMenu *BrowserWindow::createBookMarkMenu()
{
    QMenu *MarkMenu = new QMenu(tr("收藏"));
    //添加滑动页面展示已收藏网址
    // 添加一个动作来触发显示收藏网址的对话框
    QAction *showBookmarksAction = new QAction(tr("显示收藏"), this);
    connect(showBookmarksAction, &QAction::triggered, this, &BrowserWindow::showBookmarks);

    MarkMenu->addAction(showBookmarksAction);
    return MarkMenu;
}
void BrowserWindow::showBookmarks(){
    //create adialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("我的收藏"));

    QListWidget *listWidget = new QListWidget(&dialog);
    //a fuction to get all urls that is marked
    QStringList bookmarks = getAllBookmarks(); //to be continue

    for (const QString &bookmark : bookmarks) {
        listWidget->addItem(bookmark);
    }
    // 连接itemClicked信号到自定义槽
    connect(listWidget, &QListWidget::itemClicked, this, &BrowserWindow::onBookmarkClicked);
    connect(listWidget, &QListWidget::itemClicked, &dialog, &QDialog::close);


    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(listWidget);

    //add a button to close the list
    closeMarkListButton = new QPushButton(tr("关闭"), &dialog);
    connect(closeMarkListButton, &QPushButton::clicked, &dialog, &QDialog::close);
    layout->addWidget(closeMarkListButton);

    dialog.setLayout(layout);
    dialog.exec();
}
QStringList BrowserWindow::getAllBookmarks(){

    QStringList bookmarks;
    QSqlQuery query(db);
    query.exec("SELECT * FROM bookmark");
    int index = 0;
    while (query.next()) {
        index++;
        int time = query.value(1).toInt();
        QString url = query.value(0).toString();
        qDebug() << "Index:" << index << "Time:" << time << ", URL:" << url;
        bookmarks << url;
    }
    return bookmarks;
}
void BrowserWindow::onBookmarkClicked(QListWidgetItem *item) {
    if (item) {
        // 这里item->text()将返回被点击项的文本，也就是URL
        qDebug() << "Clicked URL:" << item->text();
        m_tabWidget->setUrl(QUrl::fromUserInput(item->text()));


    }
}

//always modify icon when url change
void BrowserWindow::modifyicon(){
    QSqlQuery query(db);
    query.prepare("Select * from bookmark where url = :currenturl");
    query.bindValue(":currenturl", m_urlLineEdit->text());//see if url in bookmark
    query.exec();
    if(!query.next()){//is null, means that this url is blank
        isstar = 0;
        m_starAction->setIcon(QIcon(":/images/Star0.png"));
    }
    else{
        isstar = 1;
        m_starAction->setIcon(QIcon(":/images/Star1.png"));
    }
}
void BrowserWindow::ModifyIcon(){
    modifyicon();
}


/**
 * 地址栏
 * @brief BrowserWindow::createToolBar
 * @return
 */
QToolBar *BrowserWindow::createToolBar()
{
    QToolBar *navigationBar = new QToolBar(tr("Navigation"));
    navigationBar->setMovable(false);
    navigationBar->toggleViewAction()->setEnabled(false);

    m_historyBackAction = new QAction(this);
    QList<QKeySequence> backShortcuts = QKeySequence::keyBindings(QKeySequence::Back);
    for (auto it = backShortcuts.begin(); it != backShortcuts.end();) {
        // Chromium already handles navigate on backspace when appropriate.
        if ((*it)[0] == Qt::Key_Backspace)
            it = backShortcuts.erase(it);
        else
            ++it;
    }
    // For some reason Qt doesn't bind the dedicated Back key to Back.
    backShortcuts.append(QKeySequence(Qt::Key_Back));
    m_historyBackAction->setShortcuts(backShortcuts);
    m_historyBackAction->setIconVisibleInMenu(false);
    m_historyBackAction->setIcon(QIcon(QStringLiteral(":go-previous.png")));
    m_historyBackAction->setToolTip(tr("Go back in history"));
    connect(m_historyBackAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Back);
    });
    navigationBar->addAction(m_historyBackAction);

    m_historyForwardAction = new QAction(this);
    QList<QKeySequence> fwdShortcuts = QKeySequence::keyBindings(QKeySequence::Forward);
    for (auto it = fwdShortcuts.begin(); it != fwdShortcuts.end();) {
        if (((*it)[0] & Qt::Key_unknown) == Qt::Key_Backspace)
            it = fwdShortcuts.erase(it);
        else
            ++it;
    }
    fwdShortcuts.append(QKeySequence(Qt::Key_Forward));
    m_historyForwardAction->setShortcuts(fwdShortcuts);
    m_historyForwardAction->setIconVisibleInMenu(false);
    m_historyForwardAction->setIcon(QIcon(QStringLiteral(":go-next.png")));
    m_historyForwardAction->setToolTip(tr("Go forward in history"));
    connect(m_historyForwardAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Forward);
    });
    navigationBar->addAction(m_historyForwardAction);

    m_stopReloadAction = new QAction(this);
    connect(m_stopReloadAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::WebAction(m_stopReloadAction->data().toInt()));
    });
    navigationBar->addAction(m_stopReloadAction);

    m_urlLineEdit = new QLineEdit(this);
    m_favAction = new QAction(this);
    m_urlLineEdit->addAction(m_favAction, QLineEdit::LeadingPosition);
    m_urlLineEdit->setClearButtonEnabled(true);
    navigationBar->addWidget(m_urlLineEdit);

//    auto downloadsAction = new QAction(this);
//    downloadsAction->setIcon(QIcon(QStringLiteral(":download-icon.png")));
//    downloadsAction->setToolTip(tr("Show downloads"));
//    navigationBar->addAction(downloadsAction);
//    connect(downloadsAction, &QAction::triggered, [this]() {
//        m_browser->downloadManagerWidget().show();
//    });

    //starAction is able to change its icon.
    //need to create a star list

    //database
    //数据库操作
    qDebug() << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bookmark.db");
    if( !db.open() ) //数据库打开失败
    {
        QMessageBox::warning(this, "错误", db.lastError().text());
    }
    QSqlQuery query(db);//操作
    query.exec("create table If Not EXISTS bookmark(url TEXT PRIMARY KEY, time INTEGER)");
//    query.exec("insert into bookmark(url, time) values(\"111\", 100)");
    qDebug()<<"create table successfully";

    //print the database
    query.exec("SELECT * FROM bookmark");
    int index = 0;
    while (query.next()) {
        index++;
        int time = query.value(0).toInt();
        QString url = query.value(1).toString();
        qDebug() << "Index:" << index << "Time:" << time << ", URL:" << url;
    }

    //star
    m_starAction = new QAction(this);

    //see if urlmark exist
    modifyicon();

    //click star signal
    connect(m_starAction, &QAction::triggered, [this]() {
//        m_tabWidget->triggerWebPageAction(QWebEnginePage::WebAction(m_stopReloadAction->data().toInt()));
        if(isstar==0){//not mark-->mark
            m_starAction->setIcon(QIcon(":/images/Star1.png"));
            isstar = !isstar;

            int currentTimestamp = QDateTime::currentMSecsSinceEpoch();
            QSqlQuery query(db);
            query.prepare("insert into bookmark (url, time) values(:url, :time)");
            query.bindValue(":time", currentTimestamp);
            query.bindValue(":url", m_urlLineEdit->text());//get current url
            query.exec();
//            query.exec("insert into bookmark (url, time) values (\"success\", 999)");
            qDebug()<<"mark success";
        }
        else{//mark-->not mark
            m_starAction->setIcon(QIcon(":/images/Star0.png"));
            isstar = !isstar;

            QSqlQuery query(db);
            query.prepare("delete from bookmark where url = :url");
//            query.bindValue(":time", currentTimestamp);
            query.bindValue(":url", m_urlLineEdit->text());//get current url
            query.exec();

            qDebug()<<"delete mark";
            //delete star
        }
    });
    navigationBar->addAction(m_starAction);

    return navigationBar;
}
/**
 * 页面右键刷新等动作
 * @brief BrowserWindow::handleWebActionEnabledChanged
 * @param action
 * @param enabled
 */
void BrowserWindow::handleWebActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled)
{
    switch (action) {
    case QWebEnginePage::Back:
        m_historyBackAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Forward:
        m_historyForwardAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Reload:
        m_reloadAction->setEnabled(enabled);
        modifyicon();
        break;
    case QWebEnginePage::Stop:
        m_stopAction->setEnabled(enabled);
        break;
    default:
        qWarning("Unhandled webActionChanged signal");
    }
}
/**
 * 浏览器标题处理
 * @brief BrowserWindow::handleWebViewTitleChanged
 * @param title
 */
void BrowserWindow::handleWebViewTitleChanged(const QString &title)
{
    QString suffix = m_profile->isOffTheRecord()
        ? tr("Qt Simple Browser (Incognito)")
        : tr("Qt Simple Browser");

    if (title.isEmpty())
        setWindowTitle(suffix);
    else
        setWindowTitle(title + " - " + suffix);
}
/**
 * 处理已触发的新窗口
 * @brief BrowserWindow::handleNewWindowTriggered
 */
void BrowserWindow::handleNewWindowTriggered()
{
    BrowserWindow *window = m_browser->createWindow();

    modifyicon();
    window->m_urlLineEdit->setFocus();
}
/**
 * 处理已触发的新隐私窗口
 * @brief BrowserWindow::handleNewIncognitoWindowTriggered
 */
void BrowserWindow::handleNewIncognitoWindowTriggered()
{
    BrowserWindow *window = m_browser->createWindow(/* offTheRecord: */ true);
    modifyicon();
    window->m_urlLineEdit->setFocus();
}
/**
 * 打开文件触发
 * @brief BrowserWindow::handleFileOpenTriggered
 */
void BrowserWindow::handleFileOpenTriggered()
{
    QUrl url = QFileDialog::getOpenFileUrl(this, tr("Open Web Resource"), QString(),
                                                tr("Web Resources (*.html *.htm *.svg *.png *.gif *.svgz);;All files (*.*)"));
    if (url.isEmpty())
        return;
    currentTab()->setUrl(url);
}
/**
 * 当前页面查找处理
 * @brief BrowserWindow::handleFindActionTriggered
 */
void BrowserWindow::handleFindActionTriggered()
{
    if (!currentTab())
        return;
    bool ok = false;
    QString search = QInputDialog::getText(this, tr("Find"),
                                           tr("Find:"), QLineEdit::Normal,
                                           m_lastSearch, &ok);
    if (ok && !search.isEmpty()) {
        m_lastSearch = search;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        currentTab()->findText(m_lastSearch);
#else
        currentTab()->findText(m_lastSearch, 0, [this](bool found) {
            if (!found)
                statusBar()->showMessage(tr("\"%1\" not found.").arg(m_lastSearch));
        });
#endif
    }
}
/**
 * 窗口关闭处理,最后一个标签时候弹出确认提示
 * @brief BrowserWindow::closeEvent
 * @param event
 */
void BrowserWindow::closeEvent(QCloseEvent *event)
{
    //if (m_tabWidget->count() > 1) {
        QMessageBox box(QMessageBox::Warning,tr("Confirm close"),tr("Are you sure you want to close the window ?\nThere are %1 tabs open.").arg(m_tabWidget->count()));
            box.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            box.setButtonText(QMessageBox::Ok,QString(tr("Yes")));
            box.setButtonText(QMessageBox::Cancel,QString(tr("Cancel")));
        int res = box.exec ();
        if (res == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
    //}
    event->accept();
    deleteLater();
}

TabWidget *BrowserWindow::tabWidget() const
{
    return m_tabWidget;
}
/**
 * 当前标签
 * @brief BrowserWindow::currentTab
 * @return
 */
WebView *BrowserWindow::currentTab() const
{
    return m_tabWidget->currentWebView();
}
/**
 * 页面加载进度条
 * @brief BrowserWindow::handleWebViewLoadProgress
 * @param progress
 */
void BrowserWindow::handleWebViewLoadProgress(int progress)
{
    static QIcon stopIcon(QStringLiteral(":process-stop.png"));
    static QIcon reloadIcon(QStringLiteral(":view-refresh.png"));

    if (0 < progress && progress < 100) {
        m_stopReloadAction->setData(QWebEnginePage::Stop);
        m_stopReloadAction->setIcon(stopIcon);
        m_stopReloadAction->setToolTip(tr("Stop loading the current page"));
        m_progressBar->setValue(progress);
    } else {
        m_stopReloadAction->setData(QWebEnginePage::Reload);
        m_stopReloadAction->setIcon(reloadIcon);
        m_stopReloadAction->setToolTip(tr("Reload the current page"));
        m_progressBar->setValue(0);
    }
}
/**
 * 显示窗口触发处理
 * @brief BrowserWindow::handleShowWindowTriggered
 */
void BrowserWindow::handleShowWindowTriggered()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        int offset = action->data().toInt();
        QVector<BrowserWindow*> windows = m_browser->windows();
        windows.at(offset)->activateWindow();
        windows.at(offset)->currentTab()->setFocus();
    }
}
/**
 * 开发者工具处理
 * @brief BrowserWindow::handleDevToolsRequested
 * @param source
 */
void BrowserWindow::handleDevToolsRequested(QWebEnginePage *source)
{
    source->setDevToolsPage(m_browser->createDevToolsWindow()->currentTab()->page());
    source->triggerAction(QWebEnginePage::InspectElement);
}
/**
 * 键盘按键监听
 * @brief BrowserWindow::keyPressEvent
 * @param keys
 */
void BrowserWindow::keyPressEvent(QKeyEvent *keys)
{
    //qDebug() << "keyPressEventValue:" << keys->key();

    switch (keys->key()) {
        case Qt::Key_F11:
            if(this->isFullScreen())
            {
//                this->setWindowFlags(Qt::SubWindow);
                this->showMaximized();
                toolBar->show();
                menuBar()->show();
                statusBar()->show();
            }else{
                this->showFullScreen();
                toolBar->close();
                menuBar()->hide();
                statusBar()->close();
            }
            break;
        case Qt::Key_Escape:
            //QMessageBox::about(NULL,"Key","ESC");
           break;
        case Qt::Key_F12:
            QMessageBox::about(NULL,"Key","f12");
            break;
    }
}
/**
 * 查找处理
 * */
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
void BrowserWindow::handleFindTextFinished(const QWebEngineFindTextResult &result)
{
    if (result.numberOfMatches() == 0) {
        statusBar()->showMessage(tr("\"%1\" not found.").arg(m_lastSearch));
    } else {
        statusBar()->showMessage(tr("\"%1\" found: %2/%3").arg(m_lastSearch,
                                                               QString::number(result.activeMatch()),
                                                               QString::number(result.numberOfMatches())));
    }
}
#endif
/**
 * 关于
 * @brief BrowserWindow::about
 */
void BrowserWindow::about()
{
    QMessageBox::about(this, tr("About"), tr("This openBrowser is simple browser project based on Qt webengine. "
            "It has a multi-tab page function, "
            "Mainly a dedicated client for web projects to solve browser compatibility issues."));
}
