#ifndef ASIAWEA_H
#define ASIAWEA_H

// 包含必要的头文件
#include <QMainWindow>
#include <QMap>
#include <QLabel>
#include "weatherData.h"
#include "worldlistwea.h"
#include<QNetworkAccessManager>
#include<QNetworkReply>

// 开始命名空间的声明
QT_BEGIN_NAMESPACE
namespace Ui {
class AsiaWea;  // 前向声明一个名为 AsiaWea 的类，用于用户界面对象
}
QT_END_NAMESPACE

// 声明 AsiaWea 类，它继承自 QMainWindow
class AsiaWea : public QMainWindow
{
    Q_OBJECT  // 启用 Qt 的信号和槽机制

public:
    // AsiaWea 类的构造函数，默认参数为 nullptr
    explicit AsiaWea(QWidget *parent = nullptr);
    // AsiaWea 类的析构函数
    ~AsiaWea();

    // 返回值按钮对象，类型为 WorldListWea
    WorldListWea* ListBtn = new WorldListWea;
    QString errmsg;  // 保存错误信息的字符串

protected:
    // 用于处理上下文菜单事件的函数
    void contextMenuEvent(QContextMenuEvent* event);
    // 用于处理鼠标按下事件的函数
    void mousePressEvent(QMouseEvent* event);
    // 用于处理鼠标移动事件的函数
    void mouseMoveEvent(QMouseEvent* event);
    // 获取天气数据、解析 JSON、更新 UI 的函数
    void getWeatherInfo(QString cityName);
    void parseJson(QByteArray& byteArray);
    void updateUI();
    // 重写父类的事件过滤器方法
    bool eventFilter(QObject* watched,QEvent* event);
    // 绘制高温曲线
    void paintHighCurve();
    // 绘制低温曲线
    void paintLowCurve();
    // 更新天气类型图标
    void weaTypeIcon();

private slots:
    // 用于获取数据的槽函数
    void GetData(QString);

signals:
    // 信号：用于通知主按钮事件
    void MainBtn();
    // 信号：发送温度、城市和天气类型信息
    void sendTemp(QString temp,QString city,QString type);

private:
    // 用于处理网络回复的槽函数
    void onReplied(QNetworkReply* reply);

private:
    Ui::AsiaWea *ui;  // 指向用户界面对象的指针

    QMenu* mExitMenu;   // 退出菜单的指针
    QAction* mExitAct;  // 退出菜单项的指针
    QPoint mOffset;     // 窗口移动时鼠标与左上角的偏移量

    QNetworkAccessManager* mNetAccessManager;  // 网络访问管理器的指针
    QNetworkAccessManager* mNetAccessManagerReal; // 网络访问管理器的指针（实时天气）
    QNetworkAccessManager* mNetAccessManagerAqi; // 网络访问管理器的指针（5天空气）


    Today mToday;  // 保存今日天气信息的对象
    Day mDay[5];   // 保存未来5天的天气信息的数组

    // 星期和日期的标签列表
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    // 天气类型和图标的标签列表
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    // 质量指数的标签列表
    QList<QLabel*> mAqiList;

    // 风力和风向的标签列表
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;

    QMap<QString,QString> mTypeMap;  // 用于映射天气类型的字符串映射表
};

#endif // ASIAWEA_H
