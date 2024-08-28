#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include <QApplication>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QScreen>
#include <QTranslator>


QUrl commandLineUrlArgument()//you can add a website behind this app?
{
    const QStringList args = QCoreApplication::arguments();
    // Skip the first argument (program name) and iterate through the rest
    for (const QString &arg : args.mid(1)) {
        // If the argument does not start with a dash(_), assume it's a URL
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    // If no URL is found, return a default URL
    return QUrl(QStringLiteral("https://www.baidu.com"));
}

int main(int argc, char **argv)
{
    // Set the organization name for the application, which helps identify the application properly in Windows systems
    QCoreApplication::setOrganizationName("HlinfoBrowser");

    // Set the organization domain for the application, which identifies the source or owning organization of the application
    QCoreApplication::setOrganizationDomain("hlinfo.net");

    // Enable high DPI scaling to improve display clarity on high DPI devices
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Use high resolution pixmaps to improve display quality on high DPI devices
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Support older GPU by sharing OpenGL contexts to optimize performance
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    // Attempt to use software-rendered OpenGL for compatibility with older graphics cards that do not support hardware-accelerated OpenGL
    QGuiApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    // Note: The following two lines are redundant as the OpenGL setting should typically be set in either QCoreApplication or QApplication, not both
    // QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // This line is redundant and can be removed
    // QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // This line is also redundant and can be removed

    // Create the QApplication instance, which is the basis for all Qt Widgets-based applications
    QApplication app(argc, argv);

    // Set the icon for the application's windows
    app.setWindowIcon(QIcon(QStringLiteral(":AppLogo.png")));

    // Set the application language based on the current system locale
    QLocale locale;
    QTranslator *translator = new QTranslator();
    if (locale.language() == QLocale::Chinese) {
        // If the system language is Chinese, load the Chinese translation file
        translator->load(QString(":/zh_CN.qm"));
        // Install the translator into the application for language translation
        app.installTranslator(translator);
    }


    //启用插件
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    //启用PDF预览
//    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
    //启用LocalStorage
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,true);
    //启用允许JavaScript程序读取和写入剪贴板
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,true);
    //启用JavaScript
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);

    //true允许本地加载的文档忽略跨源规则
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls,false);
    //启用全屏支持
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    //自动加载图片
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AutoLoadImages,true);
    //启用web GL
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::WebGLEnabled,true);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    //启用DNS预加载
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
#endif

    QUrl url = commandLineUrlArgument();//get url from commandline

    //create browser instance, see more in browser.cpp
    Browser browser;
    BrowserWindow *window = browser.createWindow();

    // Get the geometry
    const QRect availableGeometry = window->geometry();
    window->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    window->move((availableGeometry.width() - window->width()) / 2,(availableGeometry.height() - window->height()) / 2);
    //window->resize(1024, 768);
    window->showMaximized();
//    window->showFullScreen();
    //设置入口地址
    window->tabWidget()->setUrl(url);
    return app.exec();
}
