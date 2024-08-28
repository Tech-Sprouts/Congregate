#ifndef POEMFETCHER_H
#define POEMFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QTextStream>

class poemFetcher : public QObject {
    Q_OBJECT

public:
    explicit poemFetcher(QObject* parent = nullptr);
    QString getPoemText() const;
    void fetchPoem();  // 用于开始获取诗句的公共方法

signals:
    void poemReceived();  // 当诗句获取成功时发出信号

private slots:
    void getToken();
    void onTokenReceived();
    void getPoemWithToken();
    void onPoemReceived();

private:
    QNetworkAccessManager* manager;
    QString token;
    QString poemText;  // 存储诗句的变量
};

#endif // POEMFETCHER_H
