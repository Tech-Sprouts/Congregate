#include <iostream>
#include <map>
#include <curl/curl.h>
#include <string>

static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    // 获取到的http body存放在ptr中，先将其转换为string格式
    std::string s((char *) ptr, size * nmemb);
    std::string* result = static_cast<std::string*>(stream);
    *result += s;
    return size * nmemb;
}

std::string getCurlResult() {
    // 此处填写您在控制台-应用管理-创建应用后获取的AK
    std::string ak = "JYsRseiaDRSzSZxpX4GHlCLRX76grUic";

    // 服务地址
    // libcurl默认是不支持SSL的, 如果需要支持https，请在libcurl的编译选项中设置
    std::string host = "http://api.map.baidu.com";

    // 接口地址
    std::string uri = "/location/ip";

    // 设置请求参数
    std::map<std::string, std::string> params;
        params["ip"]     = "111.206.214.37";
        params["coor"]     = "bd09ll";
        params["ak"]        = ak;

    std::string query_str = "";
    for (auto& i : params) {
        query_str += "&" + i.first + "=" + i.second;
    }
    query_str = uri + "?" + query_str.substr(1);

    // 发送请求
    CURL *curl;
    CURLcode result_code;
    curl = curl_easy_init();
    if (curl) {
        std::string curl_result;
        std::string url = host + query_str;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        result_code = curl_easy_perform(curl);
        if (result_code != CURLE_OK) {
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(result_code) << std::endl;
            return "-1";
        }
        curl_easy_cleanup(curl);

        std::cout << "返回结果为：" << std::endl;
        std::cout << curl_result << std::endl;
        return curl_result;
    }
    return "0";
}
