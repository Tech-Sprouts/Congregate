#include "mapwidget.h"
#include "ui_mapwidget.h"
#include <QUrl>
#include <QLineEdit>
#include <QDateTime>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mychannel.h"
#include "getlnglat.cpp"

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MapWidget)
{
    ui->setupUi(this);
    // 设置web调试环境变量
    // qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");

    // 【新手bug】不要新创建QWebEngineView实例
    // 配置网页基本设置
    QWebEngineSettings *settings = ui->webView->settings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true); // 允许插件
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true); // 启用JavaScript
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true); // 允许JavaScript打开窗口

    //创建channel、c++对象并连接
    QWebChannel* webChannel = new QWebChannel(this);
    _myChannel = new myChannel(this);
    webChannel->registerObject("myObj",_myChannel);
    ui->webView->page()->setWebChannel(webChannel);

    // 绑定通道的槽函数
    QObject::connect(_myChannel,static_cast<void (myChannel::*)(qreal,qreal)>(&myChannel::signal_getlnglat), [&](qreal x,qreal y){
        ui->labelLng->setText(QString::number(x,'f',4));
        ui->labelLax->setText(QString::number(y,'f',4));
    }
    );
    //创建item对象，初始化listView
    //    _mItemModel =new QStandardItemModel(this);
    //    ui->listView->setModel(_mItemModel);

    // 加载URL
    ui->webView->load(QUrl("qrc:///new/prefix1/map.html"));


    // 创建的时候就展示通过ip定位到的经纬度
    //一开始就通过本地ip获取到经纬度
    QString curlResult = QString::fromStdString(getCurlResult());

    // 将QString转换为QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(curlResult.toUtf8());

    // 检查是否解析成功
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        // 假设我们还需要进一步解析result中的content
        if (obj.contains("content")) {
            QJsonObject contentObj = obj["content"].toObject();

            // 打印address
            if (contentObj.contains("address")) {
                qDebug() << "Address:" << contentObj["address"].toString();
                ui->curCity->setText(contentObj["address"].toString());
            }

            // 打印经纬度
            if (contentObj.contains("point")) {
                QJsonObject pointObj = contentObj["point"].toObject();
                qDebug() << "Latitude:" << pointObj["y"].toString();
                qDebug() << "Longitude:" << pointObj["x"].toString();
                ui->curLng->setText(pointObj["x"].toString());
                ui->curLax->setText(pointObj["y"].toString());

            }

        }
    } else {
        qDebug() << "Failed to parse JSON";
    }


}

MapWidget::~MapWidget()
{
    delete ui;
}




void MapWidget::on_cityLocation_clicked()
{
    // 获取输入的城市
    QString cityName = ui->lineEditCity->text().trimmed();// trimmed()去除首尾空白字符（如空格、制表符、换行符等）
    // 调用js函数
    QString jsCode = QString("cityLocation('%1')").arg(cityName);
    qDebug() << jsCode;

    ui->webView->page()->runJavaScript(jsCode,[](const QVariant &v) { qDebug() << v.toString(); });

}

void MapWidget::on_pointLocation_clicked()
{
    // 获取经纬度
    // 调用js函数
    QString jsCode = QString("pointLocation(%1,%2)").arg(ui->lineEditX->text(), ui->lineEditY->text());
    qDebug() << jsCode;
    ui->webView->page()->runJavaScript(jsCode,[](const QVariant &v) { qDebug() << v.toString(); });

}


void MapWidget::on_keyLocation_clicked()
{
    // 获取关键字
    // 调用js函数
    QString jsCode = QString("keyLocation('%1')").arg(ui->lineEditKey->text());
    qDebug() << jsCode;
    ui->webView->page()->runJavaScript(jsCode,[](const QVariant &v) { qDebug() << v.toString(); });

}



void MapWidget::on_bntReturn_clicked()
{
    // 点击返回
    QString jsCode = QString("pointLocation(%1,%2)").arg(ui->curLng->text(), ui->curLax->text());
    qDebug() << jsCode;
    ui->webView->page()->runJavaScript(jsCode,[](const QVariant &v) { qDebug() << v.toString(); });

}
