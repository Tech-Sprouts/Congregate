/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/

//not need, ignore

#include "downloadwidget.h"

#include <QFileInfo>
#include <QUrl>
#include <QWebEngineDownloadItem>

DownloadWidget::DownloadWidget(QWebEngineDownloadItem *download, QWidget *parent)
    : QFrame(parent)
    , m_download(download)
    , m_timeAdded()
{
    m_timeAdded.start();
    setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_dstName->setText(m_download->downloadFileName());
#else
    m_dstName->setText(QFileInfo(m_download->path()).fileName());
#endif

    m_srcUrl->setText(m_download->url().toDisplayString());

    connect(m_cancelButton, &QPushButton::clicked,
            [this](bool) {
        if (m_download->state() == QWebEngineDownloadItem::DownloadInProgress)
            m_download->cancel();
        else
            emit removeClicked(this);
    });

    connect(m_download, &QWebEngineDownloadItem::downloadProgress,
            this, &DownloadWidget::updateWidget);

    connect(m_download, &QWebEngineDownloadItem::stateChanged,
            this, &DownloadWidget::updateWidget);

    updateWidget();
}

// Converts the given number of bytes to a human-readable string representation
// with appropriate units (B, KiB, MiB, GiB).
// Example:
// - 123 bytes will be converted to "123 B"
// - 1024 bytes will be converted to "1.00 KiB"
// - 1048576 bytes will be converted to "1.00 MiB"
// - 1073741824 bytes will be converted to "1.00 GiB"
inline QString DownloadWidget::withUnit(qreal bytes)
{
    if (bytes < (1 << 10))
        return tr("%L1 B").arg(bytes);
    else if (bytes < (1 << 20))
        return tr("%L1 KiB").arg(bytes / (1 << 10), 0, 'f', 2);
    else if (bytes < (1 << 30))
        return tr("%L1 MiB").arg(bytes / (1 << 20), 0, 'f', 2);
    else
        return tr("%L1 GiB").arg(bytes / (1 << 30), 0, 'f', 2);
}

void DownloadWidget::updateWidget()
{
    qreal totalBytes = m_download->totalBytes();
    qreal receivedBytes = m_download->receivedBytes();
    qreal bytesPerSecond = receivedBytes / m_timeAdded.elapsed() * 1000;

    auto state = m_download->state();
    switch (state) {
    case QWebEngineDownloadItem::DownloadRequested:
        Q_UNREACHABLE();
        break;
    case QWebEngineDownloadItem::DownloadInProgress:
        if (totalBytes >= 0) {
            m_progressBar->setValue(qRound(100 * receivedBytes / totalBytes));
            m_progressBar->setDisabled(false);
            m_progressBar->setFormat(
                tr("%p% - %1 of %2 downloaded - %3/s")
                .arg(withUnit(receivedBytes))
                .arg(withUnit(totalBytes))
                .arg(withUnit(bytesPerSecond)));
        } else {
            m_progressBar->setValue(0);
            m_progressBar->setDisabled(false);
            m_progressBar->setFormat(
                tr("unknown size - %1 downloaded - %2/s")
                .arg(withUnit(receivedBytes))
                .arg(withUnit(bytesPerSecond)));
        }
        break;
    case QWebEngineDownloadItem::DownloadCompleted:
        m_progressBar->setValue(100);
        m_progressBar->setDisabled(true);
        m_progressBar->setFormat(
            tr("completed - %1 downloaded - %2/s")
            .arg(withUnit(receivedBytes))
            .arg(withUnit(bytesPerSecond)));
        break;
    case QWebEngineDownloadItem::DownloadCancelled:
        m_progressBar->setValue(0);
        m_progressBar->setDisabled(true);
        m_progressBar->setFormat(
            tr("cancelled - %1 downloaded - %2/s")
            .arg(withUnit(receivedBytes))
            .arg(withUnit(bytesPerSecond)));
        break;
    case QWebEngineDownloadItem::DownloadInterrupted:
        m_progressBar->setValue(0);
        m_progressBar->setDisabled(true);
        m_progressBar->setFormat(
            tr("interrupted: %1")
            .arg(m_download->interruptReasonString()));
        break;
    }

    if (state == QWebEngineDownloadItem::DownloadInProgress) {
        static QIcon cancelIcon(QStringLiteral(":process-stop.png"));
        m_cancelButton->setIcon(cancelIcon);
        m_cancelButton->setToolTip(tr("Stop downloading"));
    } else {
        static QIcon removeIcon(QStringLiteral(":edit-clear.png"));
        m_cancelButton->setIcon(removeIcon);
        m_cancelButton->setToolTip(tr("Remove from list"));
    }
}
