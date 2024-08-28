#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H

#include <QMap>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonValue>


class WeatherTool{ //管理区域-区域编号，可根据区域名得到区域编号

private:
    static QMap<QString,QString> mCityMaps;

    static void initCityMap(){
        QString filePath = ":/China_City_List.json";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray json = file.readAll();
        file.close();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(json,&err);
        if(err.error != QJsonParseError::NoError){
            return;
        }
        if(!doc.isArray()){
            return;
        }
        QJsonArray cities = doc.array();
        for(int i = 0;i <cities.size();i++){
            QString location_name_zh = cities[i].toObject().value("location_name_zh").toString();
            QString location_id = cities[i].toObject().value("location_id").toString();
            if(location_id.size() > 0){
                mCityMaps.insert(location_name_zh,location_id);
            }
        }
    }

public:
    static QString getLocationId(QString cityName){ // 根据区域名找到对应的区域编号
        if(mCityMaps.isEmpty()){
            initCityMap();
        }
        QMap<QString,QString>::iterator it = mCityMaps.find(cityName); // 第一次搜索

        if(it == mCityMaps.end()){ // 第一次若没有搜到，进行第二次搜索，加上“市”
            it = mCityMaps.find(cityName + "市");
        }
        if(it != mCityMaps.end()){ // 若找到了，返回区域编号
            return it.value();
        }
        return ""; // 若没找到，返回空
    }
};

QMap<QString,QString> WeatherTool::mCityMaps={};

#endif // WEATHERTOOL_H
