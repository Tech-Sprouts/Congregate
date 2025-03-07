/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#include "tabwidget.h"
#include "webpage.h"
#include "printhandler.h"
#include "webview.h"
#include "browserwindow.h"
#include <QLabel>
#include <QMenu>
#include <QTabBar>
#include <QWebEngineProfile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

TabWidget::TabWidget(QWebEngineProfile *profile, QWidget *parent)
    : QTabWidget(parent)
    , m_profile(profile)
{
    QTabBar *tabBar = this->tabBar();//get current tab

    //load qss for tab
    QFile qssFile(QStringLiteral(":tabbar.qss"));
    if(qssFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        tabBar->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }

    tabBar->setTabsClosable(true);
    //when current tab is close, choose previous tab
    tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    tabBar->setMovable(true);
    tabBar->setContextMenuPolicy(Qt::CustomContextMenu);

    //some signal
    connect(tabBar, &QTabBar::customContextMenuRequested, this, &TabWidget::handleContextMenuRequested);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &TabWidget::closeTab);
    connect(tabBar, &QTabBar::tabBarDoubleClicked, [this](int index) {
        if (index == -1)
            createTab();
        else{
            if(count()!=1){
                removeTab(index);
            }
            else{
                ;//to be continue
            }
        }
    });
    //when space double clicked, create new tab
//    connect(tabBar, &QTabBar::tabBarClicked, [this](int index){
//        BrowserWindow::ModifyIcon();
//    });

//    connect(tabBar, &QTabBar::tabBarDoubleClicked, [this](int index) {
//        if(count()!=1){//有待调整，当只剩一个标签页的时候，直接关闭浏览器
//            removeTab(index);
//        }
//        else{
////            close();
//        }
//    });
    //when a tab double clicked, delete this tab

    setDocumentMode(true);
    setElideMode(Qt::ElideRight);//when tab is too much, ignore from right

    connect(this, &QTabWidget::currentChanged, this, &TabWidget::handleCurrentChanged);

    //private mode
    if (profile->isOffTheRecord()) {
        QLabel *icon = new QLabel(this);
        QPixmap pixmap(QStringLiteral(":/images/private.png"));

        icon->setPixmap(pixmap.scaledToHeight(tabBar->height()-10));//有待调整，有点丑
        setStyleSheet(QStringLiteral("QTabWidget::tab-bar { left: %1px; }").
                      arg(icon->pixmap()->width()));
    }
}
/**
 * 当前页面变化处理
 * @brief TabWidget::handleCurrentChanged
 * @param index
 */
void TabWidget::handleCurrentChanged(int index)
{
    if (index != -1) {
        WebView *view = webView(index);//get current web tab
        if (!view->url().isEmpty())
            view->setFocus();
        emit titleChanged(view->title());
        emit loadProgress(view->loadProgress());
        emit urlChanged(view->url());
        emit favIconChanged(view->favIcon());
        emit webActionEnabledChanged(QWebEnginePage::Back, view->isWebActionEnabled(QWebEnginePage::Back));
        emit webActionEnabledChanged(QWebEnginePage::Forward, view->isWebActionEnabled(QWebEnginePage::Forward));
        emit webActionEnabledChanged(QWebEnginePage::Stop, view->isWebActionEnabled(QWebEnginePage::Stop));
        emit webActionEnabledChanged(QWebEnginePage::Reload,view->isWebActionEnabled(QWebEnginePage::Reload));
    } else {//no tab is focus, null
        emit titleChanged(QString());
        emit loadProgress(0);
        emit urlChanged(QUrl());
        emit favIconChanged(QIcon());
        emit webActionEnabledChanged(QWebEnginePage::Back, false);
        emit webActionEnabledChanged(QWebEnginePage::Forward, false);
        emit webActionEnabledChanged(QWebEnginePage::Stop, false);
        emit webActionEnabledChanged(QWebEnginePage::Reload, true);
    }
}
/**
 * 标签标题处右键菜单
 * @brief TabWidget::handleContextMenuRequested
 * @param pos
 */
//ignore, we don't have right mouse
void TabWidget::handleContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    //新建标签
    menu.addAction(tr("New &Tab"), this, &TabWidget::createTab, QKeySequence::AddTab);
    int index = tabBar()->tabAt(pos);
    if (index != -1) {
        //克隆标签
        QAction *action = menu.addAction(tr("Clone Tab"));
        connect(action, &QAction::triggered, this, [this,index]() {
            cloneTab(index);
        });
        menu.addSeparator();
        //关闭标签
        action = menu.addAction(tr("&Close Tab"));
        action->setShortcut(QKeySequence::Close);
        connect(action, &QAction::triggered, this, [this,index]() {
            closeTab(index);
        });
        //关闭其他标签
        action = menu.addAction(tr("Close &Other Tabs"));
        connect(action, &QAction::triggered, this, [this,index]() {
            closeOtherTabs(index);
        });
        menu.addSeparator();
        //刷新标签
        action = menu.addAction(tr("Reload Tab"));
        action->setShortcut(QKeySequence::Refresh);
        connect(action, &QAction::triggered, this, [this,index]() {
            reloadTab(index);
        });
    } else {
        menu.addSeparator();
    }
    //刷新所有标签
    menu.addAction(tr("Reload All Tabs"), this, &TabWidget::reloadAllTabs);
    menu.exec(QCursor::pos());
}
/**
 * 当前页面
 * @brief TabWidget::currentWebView
 * @return
 */
//no use?
WebView *TabWidget::currentWebView() const
{
    return webView(currentIndex());
}

WebView *TabWidget::webView(int index) const
{
    return qobject_cast<WebView*>(widget(index));
}

/**
 * 页面设置
 * @brief TabWidget::setupView
 * @param webView
 */
void TabWidget::setupView(WebView *webView)
{
    QWebEnginePage *webPage = webView->page();
    //show title, just show 8 char
    connect(webView, &QWebEngineView::titleChanged, [this, webView](const QString &title) {
        int index = indexOf(webView);
        if (index != -1) {
            QString subTitle = title;
            if(title.length()>8){
                subTitle = title.left(8);
            }
            setTabText(index, subTitle);
            setTabToolTip(index, title);//a tip for this tab
        }
        if (currentIndex() == index)
            emit titleChanged(title);
    });
    connect(webView, &QWebEngineView::urlChanged, [this, webView](const QUrl &url) {
        int index = indexOf(webView);
        if (index != -1)
            tabBar()->setTabData(index, url);
        if (currentIndex() == index)
            emit urlChanged(url);
    });
    connect(webView, &QWebEngineView::loadProgress, [this, webView](int progress) {
        if (currentIndex() == indexOf(webView))
            emit loadProgress(progress);
    });
    connect(webPage, &QWebEnginePage::linkHovered, [this, webView](const QString &url) {
        if (currentIndex() == indexOf(webView))
            emit linkHovered(url);
    });
    //加载favicon图标
    connect(webView, &WebView::favIconChanged, [this, webView](const QIcon &icon) {
        int index = indexOf(webView);
        if (index != -1)
            setTabIcon(index, icon);
        if (currentIndex() == index)
            emit favIconChanged(icon);
    });
    connect(webView, &WebView::webActionEnabledChanged, [this, webView](QWebEnginePage::WebAction action, bool enabled) {
        if (currentIndex() ==  indexOf(webView))
            emit webActionEnabledChanged(action,enabled);
    });
    connect(webPage, &QWebEnginePage::windowCloseRequested, [this, webView]() {
        int index = indexOf(webView);
        if (index >= 0)
            closeTab(index);
    });
    connect(webView, &WebView::devToolsRequested, this, &TabWidget::devToolsRequested);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    connect(webPage, &QWebEnginePage::findTextFinished, [this, webView](const QWebEngineFindTextResult &result) {
        if (currentIndex() == indexOf(webView))
            emit findTextFinished(result);
    });
#endif
}
/**
 * 标签标题处右键菜单之创建标签
 * @brief TabWidget::createTab
 * @return
 */
//important function--create tab
WebView *TabWidget::createTab()
{
    WebView *webView = createBackgroundTab();
    setCurrentWidget(webView);
    return webView;
}

WebView *TabWidget::createBackgroundTab()
{
    WebView *webView = new WebView;
    WebPage *webPage = new WebPage(m_profile, webView);
    webView->setPage(webPage);
    setupView(webView);
    int index = addTab(webView, tr("New Tab..."));
    setTabIcon(index, webView->favIcon());//Q
    // Workaround for QTBUG-61770
    webView->resize(currentWidget()->size());
    webView->show();
    return webView;
}
/**
 * 刷新所有标签
 * @brief TabWidget::reloadAllTabs
 */
void TabWidget::reloadAllTabs()
{
    for (int i = 0; i < count(); ++i)
        webView(i)->reload();
}
/**
 * 标签标题处右键菜单之关闭其他标签
 * @brief TabWidget::closeOtherTabs
 * @param index
 */
void TabWidget::closeOtherTabs(int index)
{
    for (int i = count() - 1; i > index; --i)
        closeTab(i);
    for (int i = index - 1; i >= 0; --i)
        closeTab(i);
}
/**
 * 标签标题处右键菜单之关闭指定标签
 * @brief TabWidget::closeTab
 * @param index
 */
void TabWidget::closeTab(int index)
{
    int tabCount = count();
    if (WebView *view = webView(index)) {
        bool hasFocus = view->hasFocus();
        if (tabCount > 1){
            removeTab(index);
        }
        if (hasFocus && tabCount > 0){
            currentWebView()->setFocus();
        }
        if (tabCount == 1){
            //createTab();
            /*QMessageBox box(QMessageBox::Warning,tr("Warm Prompt"),tr("This is the last tab. Are you sure you want to close it?"));
                box.setStandardButtons(QMessageBox::Cancel|QMessageBox::Ok);
                box.setButtonText(QMessageBox::Ok,QString(tr("Yes")));
                box.setButtonText(QMessageBox::Cancel,QString(tr("Cancel")));
            int res = box.exec ();
            if (res == QMessageBox::Ok) {
                removeTab(index);
                createTab();
                return;
            }*/
            QMessageBox::information(NULL,tr("Warm Prompt"),tr("This is the last tab. It can't be closed anymore."));
       }else{
        view->deleteLater();
        }
    }
}
/**
 * 标签标题处右键菜单之克隆标签
 * @brief TabWidget::cloneTab
 * @param index
 */
void TabWidget::cloneTab(int index)
{
    if (WebView *view = webView(index)) {
        WebView *tab = createTab();
        tab->setUrl(view->url());
    }
}

void TabWidget::setUrl(const QUrl &url)
{
    if (WebView *view = currentWebView()) {
        view->setUrl(url);
        view->setFocus();
    }
}
/**
 * 页面触发
 * @brief TabWidget::triggerWebPageAction
 * @param action
 */
void TabWidget::triggerWebPageAction(QWebEnginePage::WebAction action)
{
    if (WebView *webView = currentWebView()) {
        webView->triggerPageAction(action);
        webView->setFocus();
    }
}
/**
 * 下一个标签
 * @brief TabWidget::nextTab
 */
void TabWidget::nextTab()
{
    int next = currentIndex() + 1;
    if (next == count())
        next = 0;
    setCurrentIndex(next);
}
/**
 * 上一个标签
 * @brief TabWidget::previousTab
 */
void TabWidget::previousTab()
{
    int next = currentIndex() - 1;
    if (next < 0)
        next = count() - 1;
    setCurrentIndex(next);
}

/**
 * 刷新指定标签
 * @brief TabWidget::reloadTab
 * @param index
 */
void TabWidget::reloadTab(int index)
{
    if (WebView *view = webView(index))
        view->reload();
}
/**
 * 打印预览
 * @brief TabWidget::printPreview
 */
void TabWidget::printPreview()
{
    if (WebView *webView = currentWebView()) {
       PrintHandler handler;
       handler.setPage(webView->page());
       handler.printPreview();
    }
}
/**
 * 另存为pdf
 * @brief TabWidget::printToPDF
 */
void TabWidget::printToPDF()
{
    if (WebView *webView = currentWebView()) {
        QFileDialog fileDialog(this, tr("Export PDF"));
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
        fileDialog.setDefaultSuffix("pdf");
        if (fileDialog.exec() != QDialog::Accepted)
            return;
        QString fileName = fileDialog.selectedFiles().first();
        qDebug() << "fileName:" << fileName;
        QPageLayout layout = QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF());
       webView->page()->printToPdf(fileName,layout);
    }
//     QMessageBox::about(NULL,"打印","pdf");
}
