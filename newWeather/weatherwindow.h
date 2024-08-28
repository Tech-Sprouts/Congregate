#ifndef WEATHERWINDOW_H
#define WEATHERWINDOW_H

#include <QLabel>
#include <QMap>
#include <QMainWindow>
#include "weatherData.h"
#include "basewea.h"
#include "locationfetcher.h"
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include "citywithtemp.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WeatherWindow;
}
QT_END_NAMESPACE

class WeatherWindow : public QMainWindow {
    Q_OBJECT

public:
    WeatherWindow(QWidget* parent);
    ~WeatherWindow();

   // 五大洲的跳转按钮对象
   WorldListWea* List = new WorldListWea;
   AsiaWea* World = new AsiaWea;
   cityWithTemp* city = new cityWithTemp;

   locationFetcher* locationfetcher = new locationFetcher;


   void parseRealTimeWeather(const QByteArray &byteArray);
   void parseRealTimeAir(const QByteArray &byteArray);
   void parse5dAir(QByteArray &byteArray);
   void parseIndices(const QByteArray &byteArray);
   void parseRain(const QByteArray &byteArray);
protected:
    void contextMenuEvent(QContextMenuEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    //获取天气数据、解析Json、更新UI
    void getWeatherInfo(QString cityName);
    void parse7dWeather(QByteArray& byteArray);
    void updateUI();
    //重写父类的eventFilter方法
    bool eventFilter(QObject* watched,QEvent* event);
    void paintHighCurve();
    void paintLowCurve();
    void weaType();

signals:
    void sendTemp(QString city,QString temp,QString type);

private slots:
    void on_btnSearch_clicked();
    void receiveData(QString str);

private:
    void onReplied(QNetworkReply* reply);

private:
    Ui::WeatherWindow* ui;
    QMenu* mExitMenu;   // 退出菜单
    QAction* mExitAct;  // 菜单项（退出）
    QPoint mOffset;     //窗口移动时，鼠标与左上角的偏移
    QNetworkAccessManager* mNetAccessManager;


    Today mToday;
    Day mDay[6];

    //星期和日期
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    //天气和图标(白天)
    QList<QLabel*> mDayTypeList;
    QList<QLabel*> mDayTypeIconList;

    //天气和图标(夜晚)
    QList<QLabel*> mNightTypeList;
    QList<QLabel*> mNightTypeIconList;

    // 质量指数
    QList<QLabel*> mAqiList;

    //风力风向
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;

    QMap<QString,QString>mTypeMap;

};
#endif  // WEATHERWINDOW_H
