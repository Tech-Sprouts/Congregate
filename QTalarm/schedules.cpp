#include "schedules.h"      // 包含自定义头文件 "schedules.h"
#include "schedulemodel.h"  // 包含自定义头文件 "schedulemodel.h"
#include "fileio.h"         // 包含自定义头文件 "fileio.h"
#include <QString>          // 包含 Qt 的 QString 类，用于字符串处理
#include <QSettings>        // 包含 Qt 的 QSettings 类，用于读取和写入配置
#include <QDebug>           // 包含 Qt 的 QDebug 类，用于调试输出

// Schedules 类的构造函数，初始化父对象
Schedules::Schedules(QObject *parent) :
    QObject(parent) {}

// 加载日程数据的函数
void Schedules::LoadSchedules()
{
    FileIO *config = new FileIO(this);            // 创建一个 FileIO 对象，用于读取配置文件
    this->_Collection = config->LoadConfig();     // 调用 LoadConfig() 函数，加载配置文件中的日程数据，并赋值给 _Collection
}

// 根据索引获取特定的日程对象
ScheduleModel* Schedules::GetSchedule(int Index)
{
    return _Collection[Index];  // 返回 _Collection 列表中指定索引处的 ScheduleModel 对象
}

// 保存日程数据的函数
void Schedules::Save()
{
    FileIO *configIO = new FileIO(this);  // 创建一个 FileIO 对象，用于保存配置文件
    configIO->Save(this);                 // 调用 Save() 函数，将当前 Schedules 对象的数据保存到配置文件
}

// 获取所有日程对象的列表
QList<ScheduleModel*> Schedules::GetScheduleList()
{
    return this->_Collection;  // 返回 _Collection 列表，包含所有的 ScheduleModel 对象
}

// 添加一个新的日程到列表中
void Schedules::AddSchedule(ScheduleModel *scheToAdd)
{
    qInfo() << "Adding new alarm";               // 输出调试信息，表示正在添加新的警报
    this->_Collection.append(scheToAdd);          // 将新的 ScheduleModel 对象添加到 _Collection 列表中
}

// 根据索引移除特定的日程对象
void Schedules::removeScheduleByIndex(int index)
{
    qInfo() << "Removing alarm";                 // 输出调试信息，表示正在移除警报
    this->_Collection.removeAt(index);           // 从 _Collection 列表中移除指定索引处的 ScheduleModel 对象
}
