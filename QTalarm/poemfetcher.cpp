#include "poemfetcher.h"

poemFetcher::poemFetcher(QObject* parent)
    : QObject(parent), manager(new QNetworkAccessManager(this)) {}

void poemFetcher::fetchPoem() {
    // 检查是否存在已保存的 Token（在此示例中保存在文件中）
    QFile tokenFile("token.txt");
    if (tokenFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&tokenFile);
        token = in.readLine();
        tokenFile.close();

        // 如果 Token 存在，使用该 Token 获取诗词
        getPoemWithToken();
    } else {
        // 如果没有 Token，获取新的 Token
        getToken();
    }
}

void poemFetcher::getToken() {
    QNetworkRequest request(QUrl("https://v2.jinrishici.com/token"));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &poemFetcher::onTokenReceived);
}

void poemFetcher::onTokenReceived() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();
        token = jsonObject["data"].toString();

        // 将 Token 保存到本地存储
        QFile tokenFile("token.txt");
        if (tokenFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&tokenFile);
            out << token;
            tokenFile.close();
        }

        // 使用 Token 获取诗词
        getPoemWithToken();
    } else {
        qWarning("Failed to get token: %s", qPrintable(reply->errorString()));
    }
    reply->deleteLater();
}

void poemFetcher::getPoemWithToken() {
    QNetworkRequest request(QUrl("https://v2.jinrishici.com/sentence"));
    request.setRawHeader("X-User-Token", token.toUtf8());

    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &poemFetcher::onPoemReceived);
}

void poemFetcher::onPoemReceived() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();
        poemText = QString::fromUtf8(jsonObject["data"].toObject()["content"].toString().toUtf8());

        emit poemReceived();  // 发出诗句获取完成的信号
    } else {
        qWarning("Failed to get poem: %s", qPrintable(reply->errorString()));
    }
    reply->deleteLater();
}

QString poemFetcher::getPoemText() const {
    return poemText;
}
