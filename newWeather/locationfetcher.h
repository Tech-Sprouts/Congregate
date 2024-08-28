// locationfetcher.h

#ifndef LOCATIONFETCHER_H
#define LOCATIONFETCHER_H

#include <QString>
#include <string>

class locationFetcher {
public:
    locationFetcher();
    ~locationFetcher();

    void fetchLocation();  // 获取当前位置的方法

    QString getLatitude();
    QString getLongtitude();
private:
    QString latitude;  // 纬度
    QString longitude; // 经度

    std::string performCurlRequest();  // 执行CURL请求的方法
    static size_t curlCallback(void *ptr, size_t size, size_t nmemb, void *stream); // CURL回调函数

    void printLocation(); // 输出经纬度的方法
};

#endif // LOCATIONFETCHER_H
