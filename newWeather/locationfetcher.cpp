// locationfetcher.cpp

#include "locationfetcher.h"
#include <curl/curl.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <iostream>

// 构造函数
locationFetcher::locationFetcher() : latitude(""), longitude("") {}

// 析构函数
locationFetcher::~locationFetcher() {}

// 静态回调函数，用于处理CURL请求的返回
size_t locationFetcher::curlCallback(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::string *result = static_cast<std::string *>(stream);
    result->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

// 执行CURL请求获取位置数据
std::string locationFetcher::performCurlRequest() {
    std::string ak = "JYsRseiaDRSzSZxpX4GHlCLRX76grUic"; // 替换为你的百度API密钥
    std::string host = "http://api.map.baidu.com";
    std::string uri = "/location/ip";

    std::map<std::string, std::string> params;
    params["ip"] = "111.206.214.37";
    params["coor"] = "bd09ll";
    params["ak"] = ak;

    std::string query_str;
    for (const auto &param : params) {
        query_str += "&" + param.first + "=" + param.second;
    }
    query_str = uri + "?" + query_str.substr(1);

    CURL *curl = curl_easy_init();
    std::string curl_result;
    if (curl) {
        std::string url = host + query_str;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        CURLcode result_code = curl_easy_perform(curl);
        if (result_code != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(result_code) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return curl_result;
}

// 获取当前位置数据并解析
void locationFetcher::fetchLocation() {
    std::string curlResult = performCurlRequest();
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(curlResult).toUtf8());

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        if (obj.contains("content")) {
            QJsonObject contentObj = obj["content"].toObject();
            if (contentObj.contains("point")) {
                QJsonObject pointObj = contentObj["point"].toObject();
                latitude = pointObj["y"].toString();
                longitude = pointObj["x"].toString();
                printLocation();  // 解析成功后输出经纬度
            }
        }
    } else {
        qDebug() << "Failed to parse JSON";
    }
}

// 输出经纬度信息
void locationFetcher::printLocation() {
    qDebug() << "Latitude:" << latitude;
    qDebug() << "Longitude:" << longitude;
}

QString locationFetcher::getLatitude(){
    return latitude;
}

QString locationFetcher::getLongtitude(){
    return longitude;
}
