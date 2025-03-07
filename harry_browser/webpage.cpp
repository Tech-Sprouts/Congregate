/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#include "browserwindow.h"
#include "tabwidget.h"
#include "ui_certificateerrordialog.h"
#include "ui_passworddialog.h"
#include "webpage.h"
#include "webview.h"
#include <QAuthenticator>
#include <QMessageBox>
#include <QStyle>
#include <QTimer>
#include <QWebEngineCertificateError>

/*
 * WebPage 类提供了一个高度定制化的 QWebEnginePage 实现
 * 能够处理包括证书错误、身份验证、代理验证、功能权限请求以及协议注册等在内的各种浏览器事件
 * 通过这些处理程序，WebPage 可以增强用户在使用嵌入式浏览器时的安全性和交互性
*/

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
    connect(this, &QWebEnginePage::authenticationRequired, this, &WebPage::handleAuthenticationRequired);
    connect(this, &QWebEnginePage::featurePermissionRequested, this, &WebPage::handleFeaturePermissionRequested);
    connect(this, &QWebEnginePage::proxyAuthenticationRequired, this, &WebPage::handleProxyAuthenticationRequired);
    connect(this, &QWebEnginePage::registerProtocolHandlerRequested, this, &WebPage::handleRegisterProtocolHandlerRequested);
#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    connect(this, &QWebEnginePage::selectClientCertificate, this, &WebPage::handleSelectClientCertificate);
#endif
}
/**
 * 证书错误处理
 * @brief WebPage::certificateError
 * @param error
 * @return
 */
bool WebPage::certificateError(const QWebEngineCertificateError &error)
{
    QWidget *mainWindow = view()->window();

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QWebEngineCertificateError deferredError = error;
    deferredError.defer();

    QTimer::singleShot(0, mainWindow, [mainWindow, error = std::move(deferredError)] () mutable {
        if (!error.deferred()) {
            QMessageBox::critical(mainWindow, tr("Certificate Error"), error.errorDescription());
        } else {
#else
    if (error.isOverridable()) {
#endif
            QDialog dialog(mainWindow);
            dialog.setModal(true);
            dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

            Ui::CertificateErrorDialog certificateDialog;
            certificateDialog.setupUi(&dialog);
            certificateDialog.m_iconLabel->setText(QString());
            QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, mainWindow));
            certificateDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));
            certificateDialog.m_errorLabel->setText(error.errorDescription());
            dialog.setWindowTitle(tr("Certificate Error"));

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            if (dialog.exec() == QDialog::Accepted)
                error.ignoreCertificateError();
            else
                error.rejectCertificate();
        }
    });
    return true;
#else
        return dialog.exec() == QDialog::Accepted;
    }

    QMessageBox::critical(mainWindow, tr("Certificate Error"), error.errorDescription());
    return false;
#endif
}

/**
 * 处理身份验证
 * @brief WebPage::handleAuthenticationRequired
 * @param requestUrl
 * @param auth
 */
void WebPage::handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth)
{
    QWidget *mainWindow = view()->window();
    QDialog dialog(mainWindow);
    dialog.setModal(true);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Ui::PasswordDialog passwordDialog;
    passwordDialog.setupUi(&dialog);

    passwordDialog.m_iconLabel->setText(QString());
    QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mainWindow));
    passwordDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));

    QString introMessage(tr("Enter username and password for \"%1\" at %2")
                         .arg(auth->realm()).arg(requestUrl.toString().toHtmlEscaped()));
    passwordDialog.m_infoLabel->setText(introMessage);
    passwordDialog.m_infoLabel->setWordWrap(true);

    if (dialog.exec() == QDialog::Accepted) {
        auth->setUser(passwordDialog.m_userNameLineEdit->text());
        auth->setPassword(passwordDialog.m_passwordLineEdit->text());
    } else {
        // Set authenticator null if dialog is cancelled
        *auth = QAuthenticator();
    }
}
/**
 * 功能权限提示，如访问位置提示等
 * @brief questionForFeature
 * @param feature
 * @return
 */
inline QString questionForFeature(QWebEnginePage::Feature feature)
{
    switch (feature) {
    case QWebEnginePage::Geolocation:
        return WebPage::tr("Allow %1 to access your location information?");
    case QWebEnginePage::MediaAudioCapture:
        return WebPage::tr("Allow %1 to access your microphone?");
    case QWebEnginePage::MediaVideoCapture:
        return WebPage::tr("Allow %1 to access your webcam?");
    case QWebEnginePage::MediaAudioVideoCapture:
        return WebPage::tr("Allow %1 to access your microphone and webcam?");
    case QWebEnginePage::MouseLock:
        return WebPage::tr("Allow %1 to lock your mouse cursor?");
    case QWebEnginePage::DesktopVideoCapture:
        return WebPage::tr("Allow %1 to capture video of your desktop?");
    case QWebEnginePage::DesktopAudioVideoCapture:
        return WebPage::tr("Allow %1 to capture audio and video of your desktop?");
    case QWebEnginePage::Notifications:
        return WebPage::tr("Allow %1 to show notification on your desktop?");
    }
    return QString();
}
/**
 * 需要权限处理
 * @brief WebPage::handleFeaturePermissionRequested
 * @param securityOrigin
 * @param feature
 */
void WebPage::handleFeaturePermissionRequested(const QUrl &securityOrigin, Feature feature)
{
    QString title = tr("Permission Request");
    QString question = questionForFeature(feature).arg(securityOrigin.host());
    if (!question.isEmpty() && QMessageBox::question(view()->window(), title, question) == QMessageBox::Yes)
        setFeaturePermission(securityOrigin, feature, PermissionGrantedByUser);
    else
        setFeaturePermission(securityOrigin, feature, PermissionDeniedByUser);
}
/**
 * 代理
 * @brief WebPage::handleProxyAuthenticationRequired
 * @param auth
 * @param proxyHost
 */
void WebPage::handleProxyAuthenticationRequired(const QUrl &, QAuthenticator *auth, const QString &proxyHost)
{
    QWidget *mainWindow = view()->window();
    QDialog dialog(mainWindow);
    dialog.setModal(true);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Ui::PasswordDialog passwordDialog;
    passwordDialog.setupUi(&dialog);

    passwordDialog.m_iconLabel->setText(QString());
    QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mainWindow));
    passwordDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));

    QString introMessage = tr("Connect to proxy \"%1\" using:");
    introMessage = introMessage.arg(proxyHost.toHtmlEscaped());
    passwordDialog.m_infoLabel->setText(introMessage);
    passwordDialog.m_infoLabel->setWordWrap(true);

    if (dialog.exec() == QDialog::Accepted) {
        auth->setUser(passwordDialog.m_userNameLineEdit->text());
        auth->setPassword(passwordDialog.m_passwordLineEdit->text());
    } else {
        // Set authenticator null if dialog is cancelled
        *auth = QAuthenticator();
    }
}
/**
 * 协议处理
 * */
//! [registerProtocolHandlerRequested]
void WebPage::handleRegisterProtocolHandlerRequested(QWebEngineRegisterProtocolHandlerRequest request)
{
    auto answer = QMessageBox::question(
        view()->window(),
        tr("Permission Request"),
        tr("Allow %1 to open all %2 links?")
        .arg(request.origin().host())
        .arg(request.scheme()));
    if (answer == QMessageBox::Yes)
        request.accept();
    else
        request.reject();
}
//! [registerProtocolHandlerRequested]

/**
 * 客户端证书的支持
 * */
#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void WebPage::handleSelectClientCertificate(QWebEngineClientCertificateSelection selection)
{
    // Just select one.
    selection.select(selection.certificates().at(0));
}
#endif
