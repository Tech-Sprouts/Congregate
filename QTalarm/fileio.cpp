#include "fileio.h"
#include "schedulemodel.h"
#include "schedules.h"
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QDateTime>
#include <QTime>
#include <QList>
#include <QDebug>

// FileIO 类的构造函数，继承自 QObject 类
FileIO::FileIO(QObject *parent) :
    QObject(parent)
{
    // 构造函数体为空，暂不需要额外初始化
}

// 提取音频文件到临时目录
bool FileIO::ExtractAudio()
{
    // 从资源文件中复制音频文件到操作系统的临时目录
    if (QFile::copy(":/new/sounds/alarm.ogg", QDir::tempPath() + "/QTalarm.ogg"))
    {
        // 如果复制成功，返回 true 表示操作成功
        return true;
    }
    // 如果复制失败，返回 false 表示操作失败
    return false;
}

// 删除提取到临时目录中的音频文件
bool FileIO::DelExtracted()
{
    // 尝试删除指定的音频文件，如果删除成功，返回 true，否则返回 false
    return QFile::remove(QDir::tempPath() + "/QTalarm.ogg");
}

// 加载配置文件中的所有警报设置
QList<ScheduleModel*> FileIO::LoadConfig()
{
    qDebug() << QSettings().fileName();

    QList<ScheduleModel*> scheduleList; // 创建一个空的警报模型列表，用于存储加载的警报配置
    QString indexStr; // 用于在循环中生成每个警报的键的字符串

    qInfo() << "开始加载配置"; // 输出调试信息

    // 根据保存的警报数量循环加载每个警报的配置
    for (int index = 0; index < this->_Settings.value("AlarmCount").toInt(); index++)
    {
        ScheduleModel *sched = new ScheduleModel(this); // 为每个警报创建一个新的 ScheduleModel 对象
        indexStr.setNum(index); // 设置当前警报的索引字符串

        // 从配置中加载警报时间，如果没有设置时间，则将时间初始化为 00:00:00
        sched->AlarmTime = this->_Settings.value(indexStr + "Time").toTime();
        if (sched->AlarmTime.isNull())
        {
            QTime reset; // 创建一个 QTime 对象
            reset.setHMS(0, 0, 0, 0); // 将时间设置为午夜（00:00:00.000）
            sched->AlarmTime = reset; // 将未设置的时间初始化为默认时间
        }

        // 从配置中加载每周的启用状态
        sched->isMonEnabled = this->_Settings.value(indexStr + "MonEnabled").toBool();
        sched->isTueEnabled = this->_Settings.value(indexStr + "TueEnabled").toBool();
        sched->isWedEnabled = this->_Settings.value(indexStr + "WedEnabled").toBool();
        sched->isThurEnabled = this->_Settings.value(indexStr + "ThurEnabled").toBool();
        sched->isFriEnabled = this->_Settings.value(indexStr + "FriEnabled").toBool();
        sched->isSatEnabled = this->_Settings.value(indexStr + "SatEnabled").toBool();
        sched->isSunEnabled = this->_Settings.value(indexStr + "SunEnabled").toBool();
        sched->isBastard = this->_Settings.value(indexStr + "Bastard").toBool();

        // 加载自定义警报的启用状态和日期
        sched->isCustomAlarmEnabled = this->_Settings.value(indexStr + "CustEnabled").toBool();
        sched->CustomAlarm = this->_Settings.value(indexStr + "CustDate").toDate();

        // 加载自定义声音的启用状态和路径
        sched->isCustomSoundEnabled = this->_Settings.value(indexStr + "CustomSoundEnabled").toBool();
        sched->CustomSoundPath = this->_Settings.value(indexStr + "CustomSound").toString();
        sched->Index = index; // 设置警报的索引
        sched->isOneshot = this->_Settings.value(indexStr + "isOneshot").toBool(); // 加载是否为一次性警报的设置

        scheduleList.append(sched); // 将加载的警报模型添加到警报列表中
    }
    return scheduleList; // 返回加载的警报模型列表
}

// 将警报配置保存到配置文件
bool FileIO::Save(Schedules *Collection)
{
    try
    {
        QList<ScheduleModel*> SchedList = Collection->GetScheduleList(); // 获取警报列表
        ScheduleModel *currentSche; // 当前警报对象
        int index = 0; // 初始化警报索引

        this->_Settings.setValue("AlarmCount", SchedList.count()); // 保存警报数量到配置文件
        qInfo() << "开始保存配置"; // 输出调试信息

        // 遍历每个警报对象并保存其设置到配置文件
        foreach (currentSche, SchedList)
        {
            QString IndexStr;
            IndexStr.setNum(index); // 设置当前警报的索引字符串

            // 保存每个警报的启用状态、时间、日期、声音等配置
            this->_Settings.setValue(IndexStr + "MonEnabled", currentSche->isMonEnabled);
            this->_Settings.setValue(IndexStr + "TueEnabled", currentSche->isTueEnabled);
            this->_Settings.setValue(IndexStr + "WedEnabled", currentSche->isWedEnabled);
            this->_Settings.setValue(IndexStr + "ThurEnabled", currentSche->isThurEnabled);
            this->_Settings.setValue(IndexStr + "FriEnabled", currentSche->isFriEnabled);
            this->_Settings.setValue(IndexStr + "SatEnabled", currentSche->isSatEnabled);
            this->_Settings.setValue(IndexStr + "SunEnabled", currentSche->isSunEnabled);
            this->_Settings.setValue(IndexStr + "Bastard", currentSche->isBastard);
            this->_Settings.setValue(IndexStr + "Time", currentSche->AlarmTime);
            this->_Settings.setValue(IndexStr + "CustEnabled", currentSche->isCustomAlarmEnabled);
            this->_Settings.setValue(IndexStr + "CustDate", currentSche->CustomAlarm);
            this->_Settings.setValue(IndexStr + "CustomSoundEnabled", currentSche->isCustomSoundEnabled);
            this->_Settings.setValue(IndexStr + "CustomSound", currentSche->CustomSoundPath);
            this->_Settings.setValue(IndexStr + "isOneshot", currentSche->isOneshot);
            this->_Settings.sync(); // 同步配置到磁盘，确保数据保存
            index++; // 增加索引以处理下一个警报
        }
    }
    catch (...)
    {
        // 捕获所有异常，返回 false 表示保存失败
        return false;
    }

    // 如果保存成功，返回 true
    return true;
}

// 加载音量设置
int FileIO::LoadVolume()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    return settings.value("Volume").toInt(); // 返回配置文件中的音量值
}

// 保存音量设置
void FileIO::SaveVolume(int vol)
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    settings.setValue("Volume", vol); // 将音量值保存到配置文件
}

// 加载窗口显示设置
bool FileIO::LoadWindowShow()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    if (!settings.contains("ShowWindowDefault"))
    {
        // 如果配置文件中没有找到 "ShowWindowDefault" 设置，返回 true 表示默认显示窗口
        return true;
    }
    // 返回配置文件中的 "ShowWindowDefault" 设置值
    return settings.value("ShowWindowDefault").toBool();
}

// 保存窗口显示设置
void FileIO::SaveWindowShow(bool showWindow)
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    settings.setValue("ShowWindowDefault", showWindow); // 将窗口显示设置保存到配置文件
}

// 检查是否使用 24 小时制时间显示
bool FileIO::isMilTime()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    return settings.value("isMilTime").toBool(); // 返回配置文件中的时间制设置
}

// 加载是否为单声道设置
bool FileIO::LoadisMono()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    return settings.value("isMono").toBool(); // 返回配置文件中的单声道设置
}

// 保存是否为单声道设置
void FileIO::SaveisMono(bool isMono)
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    settings.setValue("isMono", isMono); // 将单声道设置保存到配置文件
}

// 保存是否使用 24 小时制时间显示
void FileIO::SaveisMilTime(bool isMilTime)
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    settings.setValue("isMilTime", isMilTime); // 将时间制设置保存到配置文件
}

// 加载是否在 PM 时间警告
bool FileIO::LoadWarnOnPm()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    return settings.value("WarnOnPm").toBool(); // 返回配置文件中的 PM 时间警告设置
}

// 加载是否已查看解决提示文本
bool FileIO::LoadSeenSolveText()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    return settings.value("SeenSolvedText").toBool(); // 返回配置文件中的已查看解决提示文本设置
}

// 保存是否在 PM 时间警告
void FileIO::SaveWarnOnPm(bool warn)
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    settings.setValue("WarnOnPm", warn); // 将 PM 时间警告设置保存到配置文件
}

// 保存已查看解决提示文本状态
void FileIO::SaveSeenSolveText()
{
    QSettings settings; // 创建 QSettings 对象以访问配置文件
    settings.setValue("SeenSolvedText", true); // 将已查看解决提示文本设置保存到配置文件，标记为 true
}
