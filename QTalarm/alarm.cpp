#include "alarm.h" // 包含自定义头文件 "alarm.h"
#include "fileio.h" // 包含自定义头文件 "fileio.h"
#include <QMediaPlayer> // 包含 QMediaPlayer 类的头文件，用于音频播放
#include <QString> // 包含 QString 类的头文件，用于字符串处理
#include <QDir> // 包含 QDir 类的头文件，用于处理目录路径

// 构造函数，初始化 Alarm 类的实例
Alarm::Alarm(QObject *parent) :
    QObject(parent) // 调用父类 QObject 的构造函数
{
    media = new QMediaPlayer(this); // 创建 QMediaPlayer 对象，用于播放音频

    this->_DefaultPath = QDir::tempPath() + "/QTalarm.ogg"; // 设置默认音频文件路径为临时目录下的 "QTalarm.ogg"
    this->_isPlaying = false; // 初始化播放状态为 false，表示未播放
    this->_Pause = new QTimer(this); // 创建 QTimer 对象，用于暂停控制
    this->canResume = true; // 初始化 canResume 为 true，表示可以恢复播放

    connect(this->_Pause, SIGNAL(timeout()), this, SLOT(Resume())); // 连接定时器超时信号到 Resume() 槽函数，用于恢复播放
}

// 启动警报音频的播放
void Alarm::Start(bool useCustom)
{
    qInfo() << "Starting alarm audio"; // 输出调试信息，表明音频开始播放
    if(useCustom) // 如果使用自定义路径
    {
        media->setMedia(QUrl::fromLocalFile(this->_CustPath)); // 设置媒体为自定义音频文件
        this->UsingCustomPath = true; // 标记使用自定义路径
    } else {
        FileIO::ExtractAudio(); // 调用 FileIO 类的静态方法 ExtractAudio() 提取默认音频文件
        media->setMedia(QUrl::fromLocalFile(this->_DefaultPath)); // 设置媒体为默认音频文件
        this->UsingCustomPath = false; // 标记使用默认路径
    }
    media->play(); // 开始播放音频
    connect(media, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(RepeatAllTheThings(QMediaPlayer::MediaStatus))); // 连接媒体状态改变信号到 RepeatAllTheThings 槽函数
    this->_isPlaying = true; // 设置播放状态为 true，表示正在播放
}

// 停止警报音频的播放
void Alarm::Stop()
{
    qInfo() << "Stopping alarm audio"; // 输出调试信息，表明音频停止播放
    media->stop(); // 停止音频播放
    this->_Pause->start(60000); // 启动暂停定时器，设定1分钟后尝试恢复播放
    this->_isPlaying = false; // 设置播放状态为 false，表示未播放
}

// 检查媒体状态并重复播放音频
void Alarm::RepeatAllTheThings(QMediaPlayer::MediaStatus state)
{
    if(state == QMediaPlayer::EndOfMedia) // 如果媒体播放结束
    {
        if(this->UsingCustomPath) // 如果使用自定义路径
        {
            media->setMedia(QUrl::fromLocalFile(this->_CustPath)); // 重新设置媒体为自定义音频文件
        } else {
            media->setMedia(QUrl::fromLocalFile(this->_DefaultPath)); // 重新设置媒体为默认音频文件
        }
        media->play(); // 重新开始播放音频
    }
}

// 返回当前音频是否正在播放
bool Alarm::isPlaying()
{
    return this->_isPlaying; // 返回 _isPlaying 的值
}

// 设置自定义音频文件路径
void Alarm::SetCustomPath(QString CustPath)
{
    this->_CustPath = CustPath; // 设置自定义路径
}

// 恢复音频播放
void Alarm::Resume()
{
    this->canResume = true; // 标记可以恢复播放
    this->_Pause->stop(); // 停止暂停定时器
}

// 设置音量
void Alarm::SetVolume(int Volume)
{
    media->setVolume(Volume); // 设置媒体播放器的音量
    FileIO::SaveVolume(Volume); // 调用 FileIO 类的静态方法 SaveVolume() 保存音量设置
}
