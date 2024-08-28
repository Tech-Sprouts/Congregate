#ifndef ASIALISTWEA_H
#define ASIALISTWEA_H


// 包含必要的头文件
#include <QMainWindow>
#include <QLabel>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidget>
#include "citywithtemp.h"

namespace Ui {
class AsiaListWea;  // 前向声明一个名为 AsiaListWea 的类，用于用户界面对象
}

// 定义 AsiaListWea 类，它继承自 QMainWindow
class AsiaListWea : public QMainWindow
{
    Q_OBJECT  // 启用 Qt 的信号和槽机制

public:
    // AsiaListWea 类的构造函数，默认参数为 nullptr
    explicit AsiaListWea(QWidget *parent = nullptr);
    // AsiaListWea 类的析构函数
    ~AsiaListWea();

    // 成员变量，用于保存更新后的城市、温度和天气类型信息
    QString updateCity;
    QString updateTemp;
    QString updateType;

    // 成员变量，保存城市、温度、天气类型和点击的城市列表
    QList<QString> cityList;
    QList<QString> tempList;
    QList<QString> typeList;
    QList<QString> clickedCity;

protected:
    // 用于处理上下文菜单事件的函数
    void contextMenuEvent(QContextMenuEvent* event);
    // 用于处理鼠标按下事件的函数
    void mousePressEvent(QMouseEvent* event);
    // 用于处理鼠标移动事件的函数
    void mouseMoveEvent(QMouseEvent* event);

    // 设置状态的函数
    void setState();
    // 添加亚洲城市列表的函数
    void asiaCity();
    // 添加欧洲城市列表的函数
    void euCity();
    // 添加非洲城市列表的函数
    void afCity();
    // 添加北美洲城市列表的函数
    void naCity();
    // 添加大洋洲城市列表的函数
    void oaCity();

signals:
    // 信号：返回按钮事件
    void BackButton();
    // 信号：设置数据
    void setData(QString text);
    // 信号：发送岛屿信息
    void sendIsland(QString);

private slots:
    // 槽函数：处理按钮点击事件
    void on_pushButton_2_clicked();
    // 槽函数：处理列表项双击事件
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    // 槽函数：处理状态窗口列表项双击事件
    void on_stateWidget_itemDoubleClicked();
    // 槽函数：处理列表按钮点击事件
    void on_listBtn_clicked();
    // 槽函数：获取温度信息
    void getTemp(QString temp, QString city, QString type);

private:
    Ui::AsiaListWea *ui;  // 指向用户界面对象的指针
    QMenu* mExitMenu;       // 退出菜单的指针
    QAction* mExitAct;      // 退出菜单项的指针
    QPoint mOffset;         // 窗口移动时鼠标与左上角的偏移量
    QNetworkAccessManager* mNetAccessManager;  // 网络访问管理器的指针

    // 添加项到列表的私有方法
    void AddItem(QString city, QString temp, QString pic);
};


#endif // ASIALISTWEA_H
