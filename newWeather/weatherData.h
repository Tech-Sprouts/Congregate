#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include<QString>

class Today{

public:
    Today()
    {
        fxdate = "2024-08-22";
        location = "北京";

        moonPhase = "感冒指数";
        temperature = 0;
        humidity = "0%";
        pm25 = 0;
        category = "无数据";

        type = "多云";
        icon = "-";
        windScale = "2级";
        windDir = "南风";
        high = 20;
        low = 18;

        sport = "-";
        washCar = "-";

        rainText = "-";
    }
    QString fxdate;
    QString location;

    QString moonPhase;
    QString humidity;

    double temperature;
    double pm25;

    QString category;
    QString type;
    QString icon;

    QString windScale;
    QString windDir;

    int high;
    int low;

    QString sport;
    QString washCar;

    QString rainText;
};

class Day{
public:
    Day(){
        fxdate = "2024-08-24";
        week = "周六";

        textDay = "多云";
        textNight = "晴";

        tempMax = 0;
        tempMin = 0;

        windscaleDay = "2级";
        windDirDay = "南风";

        category = "-";
        moonPhase = "-";
    }
    QString fxdate;
    QString week;

    QString textDay;
    QString textNight;

    int tempMax;
    int tempMin;

    QString windscaleDay;
    QString windDirDay;

    QString category;
    QString moonPhase;
};

#endif // WEATHERDATA_H
