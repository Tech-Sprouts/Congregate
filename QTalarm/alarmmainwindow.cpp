#include "alarmmainwindow.h"
#include "ui_alarmmainwindow.h"
#include "timer.h"
#include "alarm.h"
#include "fileio.h"
#include "schedules.h"
#include "aboutdialog.h"
#include "settingdialog.h"
#include "snooze.h"
#include "bastardsnooze.h"
//#include "clockwidget.h"
#include "waterprogressbar.h"
#include "poemfetcher.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QCloseEvent>
#include <QTimeEdit>
#include <QTimer>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QSlider>
#include <QSystemTrayIcon>
#include <QListWidgetItem>
#include <QCalendarWidget>
#include <QToolTip>
#include <QLocalSocket>
#include <QLocalServer>
#include <myclockwidgetfinal.h>
#include <QTextCharFormat>


alarmMainWindow::alarmMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::alarmMainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/new/icons/Icons/clockSmall.png"));
    this->_supportsTray = QSystemTrayIcon::isSystemTrayAvailable();
    if(!this->_supportsTray)
    {
        qInfo() << "没有找到系统托盘。";
    }

    TerminateIfRunning();


    poemfetcher  = new poemFetcher();

    // 连接信号和槽，以在诗句获取后更新 QLabel
    connect(poemfetcher, &poemFetcher::poemReceived, this, [=]() {
        // 使用 getPoemText() 获取诗句文本，并设置到 QLabel 上
        ui->lblPoem->setText(poemfetcher->getPoemText());
    });

    // 启动诗句获取流程
    poemfetcher->fetchPoem();

    //add buttons
    ConfigureButtons();

    //Create / load Schedule
    _Schedules=new Schedules(this);
    _Schedules->LoadSchedules();
    PopulateListWidget();
    DisablePanelIfNoSelection();
    if(ui->listWidget->currentRow()==0)
        ShowActiveAlarm(0);

    LoadOptions();
    toggleOnDuplicateProcess = QApplication::arguments().count() > 1 ? QApplication::arguments().at(1) == "--toggleOnDup":false;


    // 设置线程
    TimeKeeper = new Timer(this, _Schedules); // 创建 Timer 对象 TimeKeeper，用于时间管理和多线程操作，传入当前对象（this）和 _Schedules 对象作为参数

    // 设置时钟单例
    CurAlarm = &Alarm::GetInstance(); // 获取 Alarm 单例对象的引用，使用静态方法 GetInstance()，确保只存在一个 Alarm 实例，并将其指针赋值给 CurAlarm

    TimeKeeper->StartTimer(CurAlarm); // 调用 TimeKeeper 对象的 StartTimer() 方法，启动计时器并传入 CurAlarm 对象，用于管理警报的时间控制


    SetupVolume();

    //Tray icons
    QAction *QAshow=new QAction("&Show",this);
    QAction *QAquit=new QAction("&Quit",this);
    SetupTrayIcon(QAshow,QAquit);


    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);//去掉列表头
    //设置星期风格
    QTextCharFormat format;
//    format.setForeground(QColor(5, 15, 255, 187)); // 设置前景色为半透明的蓝色
    format.setForeground(QColor(255, 255, 255, 255)); // 设置前景色为白色
//    format.setBackground(QColor(247, 247, 247, 100)); // 设置背景色为半透明的浅灰色
//    format.setBackground(QColor(255, 255, 255, 20)); // 设置背景色为半透明的浅白色
    format.setBackground(QColor(31,53,114, 180));
    format.setFontFamily("Microsoft YaHei");
    format.setFontPointSize(15);
    format.setFontWeight(QFont::Medium);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Saturday, format);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Sunday, format);
    format.setForeground(QColor(200, 190, 190, 255)); // 设置前景色为半透明的黑色
//    format.setBackground(QColor(255, 255, 255, 30)); // 设置背景色为半透明的浅白色
    ui->calendarWidget->setWeekdayTextFormat(Qt::Monday, format);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Tuesday, format);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Wednesday, format);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Thursday, format);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Friday, format);


    ui->txtSoundPath->setText(""); // 清空 txtSoundPath 文本框的内容，将其设置为空字符串
    ui->CustEdit->setDate(QDate::currentDate()); // 将 CustEdit 日期选择器的日期设置为当前日期
    SetupClock(); // 调用 SetupClock() 函数，设置或初始化时钟（具体功能取决于函数实现）
    ui->chkBastard->setToolTip("仅在解答一个随机数学问题后才能停止警报!"); // 设置 chkBastard 复选框的提示文本
    ui->chkSounds->setToolTip("使用自定义的声音/视频文件作为唤醒音"); // 设置 chkSounds 复选框的提示文本



    SetupSlots(QAquit,QAshow);
}

alarmMainWindow::~alarmMainWindow()
{
    delete ui;
}

void alarmMainWindow::closeEvent(QCloseEvent * event)
{
    event->ignore();
    if(this->_supportsTray)
    {
        this->hide();
    }else{
        Quit();
    }
}

//void MainWindow::SetupClock()
//{
//    // 创建网络管理器，用于处理HTTP请求
//    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

//    // 连接信号和槽，以处理请求完成后的响应
//    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::handleTimeResponse);

//    // 设置定时器，每隔500毫秒请求一次时间
//    QTimer* CurrentTime = new QTimer(this);
//    connect(CurrentTime, &QTimer::timeout, this, [=]() {
//        QNetworkRequest request(QUrl("http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp"));
//        manager->get(request); // 发送GET请求
//    });

//    CurrentTime->start(500); // 每500毫秒触发一次timeout信号
//}

void alarmMainWindow::SetupClock()
{
    // 创建网络管理器，用于处理HTTP请求
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    // 连接信号和槽，以处理请求完成后的响应
    connect(manager, &QNetworkAccessManager::finished, this, &alarmMainWindow::handleTimeResponse);

    // 设置定时器，每隔500毫秒请求一次时间
    QTimer* CurrentTime1 = new QTimer(this);
    connect(CurrentTime1, &QTimer::timeout, this, [=]() {
        QNetworkRequest request(QUrl("http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp")); // 淘宝API调用时间
        manager->get(request); // 发送GET请求
    });

//    connect(CurrentTime, &QTimer::timeout,this,&MainWindow::timeCheck);

    //设置时钟显示
    QTimer *CurrentTime=new QTimer(this);
    connect(CurrentTime,SIGNAL(timeout()),this,SLOT(timeCheck()));
    CurrentTime->start(500);



    CurrentTime1->start(100); // 每100毫秒触发一次timeout信号
}

void alarmMainWindow::handleTimeResponse(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        // 读取响应数据并解析JSON
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        // 提取时间戳数据
        QJsonObject dataObject = jsonObject["data"].toObject();
        QString timestampString = dataObject["t"].toString();

        // 将时间戳转换为 QDateTime 对象
        qint64 timestamp = timestampString.toLongLong();
        this->currentTime = QDateTime::fromMSecsSinceEpoch(timestamp);

        // 分别提取日期和时间
        QString dateStr = currentTime.toString("yyyy-MM-dd"); // 获取日期字符串
        QString timeStr = currentTime.toString("hh:mm:ss");   // 获取时间字符串

        // 更新界面上的日期和时间显示
        ui->dateLabel->setText(dateStr); // 更新日期标签
//        ui->Clock->setText(timeStr); // 更新时间标签
    } else {
        // 处理错误情况
        qDebug() << "Error occurred:" << reply->errorString();
    }

    reply->deleteLater(); // 完成后删除reply对象
}


void alarmMainWindow::ToggleWindow(QSystemTrayIcon::ActivationReason Reason)
{
    if(Reason==QSystemTrayIcon::DoubleClick || Reason==QSystemTrayIcon::Trigger)
    {
        ToggleWindow();
    }
}

void alarmMainWindow::ToggleWindow() // 定义 MainWindow 类的成员函数 ToggleWindow，用于切换窗口显示状态
{
    // 检查当前警报是否正在播放，并且不是“Bastard”模式
    if(this->CurAlarm->isPlaying() && this->CurAlarm->isBastard == false)
    {
        this->CurAlarm->Stop(); // 如果满足条件，则停止当前警报
    }
    ui->TestBtn->setText("测试");
    // 检查当前窗口是否隐藏
    if(this->isHidden())
    {
        this->show(); // 如果窗口当前是隐藏的，则显示窗口
    } else {
        this->hide(); // 如果窗口当前是显示的，则隐藏窗口
    }
    UpdateListWidget(); // 调用 UpdateListWidget() 函数，更新列表控件的显示
}

void alarmMainWindow::SetTime() // 定义了 MainWindow 类中的一个名为 SetTime 的函数
{
    // 如果没有选择行，可能是回车被按下，时间被记录，然后触发了更改，
    // 除非这些更改已经被记录
    if(ui->listWidget->currentIndex().column()!=-1) // 检查 listWidget 中是否有选中的行
    {
        // 如果时间超过12点，并且不是24小时制，而且需要在下午时间时进行提醒
        if(ui->timeEdit->time().hour()>12 && !_isMilTime && _WarnOnPm)
        {
            PMWarning(); // 调用 PMWarning() 函数，发出提醒
        }

        // 获取当前选择行对应的 ScheduleModel 对象
        ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());

        // 将选择的时间控件（timeEdit）中的时间设置为当前 ScheduleModel 的 AlarmTime
        Active->AlarmTime = ui->timeEdit->time();

        UpdateListWidget(); // 更新列表控件
        this->_Schedules->Save(); // 保存当前日程表的数据
    }
}


void alarmMainWindow::SetCustomDate()
{
    if(ui->listWidget->currentIndex().column()!=-1)
    {
        //Update date on the display
        ui->CustEdit->setDate(ui->calendarWidget->selectedDate());
        ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        Active->AlarmTime = ui->timeEdit->time();
        QDate CustomDate=ui->calendarWidget->selectedDate();
        Active->CustomAlarm = CustomDate;
        if(Active->isCustomAlarmEnabled)
            UpdateListWidget();
        this->_Schedules->Save();
    }
}


void alarmMainWindow::ToggleBastard(bool val) // 定义 MainWindow 类的成员函数 ToggleBastard，用于切换“Bastard”模式
{
    // 获取当前选中行对应的 ScheduleModel 对象
    ScheduleModel *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());

    Active->isBastard = val; // 设置当前 ScheduleModel 对象的 isBastard 属性为传入的布尔值 val

    UpdateListWidget(); // 调用 UpdateListWidget() 函数，更新列表控件的显示

    this->_Schedules->Save(); // 调用 _Schedules 对象的 Save() 方法，保存日程数据

    SendTrayMessage(solveNotificationTitle, solveNotification); // 调用 SendTrayMessage() 函数，发送系统托盘消息，通知用户“Bastard”模式的变更
}


void alarmMainWindow::ToggleMon(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isMonEnabled = isEnabled;
    UpdateListWidget();
}

void alarmMainWindow::ToggleTue(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isTueEnabled = isEnabled;
    UpdateListWidget();
}
void alarmMainWindow::ToggleWed(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isWedEnabled = isEnabled;
    UpdateListWidget();
}
void alarmMainWindow::ToggleThur(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isThurEnabled = isEnabled;
    UpdateListWidget();
}
void alarmMainWindow::ToggleFri(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isFriEnabled = isEnabled;
    UpdateListWidget();
}
void alarmMainWindow::ToggleSat(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isSatEnabled = isEnabled;
    UpdateListWidget();
}
void alarmMainWindow::ToggleSun(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isSunEnabled = isEnabled;
    UpdateListWidget();
}


void alarmMainWindow::ToggleCust(bool isEnabled)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isCustomAlarmEnabled = isEnabled;
    UpdateListWidget();
}

void alarmMainWindow::ToggleOneshot(bool isEnabled)
{
    ScheduleModel *Active = this->_Schedules->GetSchedule(this->ui->listWidget->currentRow());
    Active->isOneshot = isEnabled;
    if(isEnabled)
    {
        DisableGUIIfOneshot(true);
    }else{
        DisableGUIIfOneshot(false);
    }
    UpdateListWidget();
}

void alarmMainWindow::Quit() // 定义 MainWindow 类的成员函数 Quit，用于退出应用程序
{
    // 检查当前警报是否正在播放，并且是“Bastard”模式
    if(this->CurAlarm->isPlaying() && this->CurAlarm->isBastard)
    {
        // 消息框将导致应用程序退出，即使我们告诉它不要退出（注释掉的代码）
        // QMessageBox::warning(this,"Nope","you didn't think it was going to be that easy did you?");
        return; // 如果条件成立，直接返回，不执行后续退出操作
    } else {
        this->_Schedules->Save(); // 调用 _Schedules 对象的 Save() 方法，保存所有计划数据
        FileIO::DelExtracted(); // 调用 FileIO 的静态方法 DelExtracted() 删除已提取的文件（通常是临时文件）
        FileIO::SaveVolume(ui->VolumeSlider->value()); // 调用 FileIO 的静态方法 SaveVolume() 保存当前音量滑块的值
        qApp->quit(); // 调用 Qt 全局应用程序对象的 quit() 方法，退出应用程序
    }
}


void alarmMainWindow::AddRemoveAlarm(QAbstractButton *button)
{
    if(button->text()=="&Add")
    {
        ScheduleModel *scheToAdd=new ScheduleModel(this);
        this->_Schedules->AddSchedule(scheToAdd);
        ui->listWidget->addItem(scheToAdd->Name());
        int listIndex = ui->listWidget->count()-1;
        scheToAdd->Index = listIndex;
        ui->listWidget->setCurrentRow(listIndex);
    }
    else if(button->text()=="&Remove")
    {
        this->_lastDeletedIndex=ui->listWidget->currentRow();
        this->_Schedules->removeScheduleByIndex(ui->listWidget->currentRow());
        PopulateListWidget();
    }
    else if(button->text()=="Clone")
    {
        ScheduleModel *toClone = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        ScheduleModel *cloned = new ScheduleModel(this);
        cloned->CustomAlarm = toClone->CustomAlarm;
        cloned->isCustomAlarmEnabled = toClone->isCustomAlarmEnabled;
        cloned->isCustomSoundEnabled = toClone->isCustomSoundEnabled;
        cloned->isFriEnabled = toClone->isFriEnabled;
        cloned->isMonEnabled = toClone->isMonEnabled;
        cloned->isSatEnabled = toClone->isSatEnabled;
        cloned->isSunEnabled = toClone->isSunEnabled;
        cloned->isThurEnabled = toClone->isThurEnabled;
        cloned->isTueEnabled = toClone->isTueEnabled;
        cloned->isWedEnabled = toClone->isWedEnabled;
        cloned->isBastard = toClone->isBastard;
        cloned->AlarmTime = toClone->AlarmTime;
        cloned->CustomSoundPath = toClone->CustomSoundPath;
        this->_Schedules->AddSchedule(cloned);
        PopulateListWidget();
    }
    this->_Schedules->Save();
}



void alarmMainWindow::ShowActiveAlarm(int index)
{
    if(!ui->listAlmBtn->button(QDialogButtonBox::Cancel)->isEnabled())
        ui->listAlmBtn->button(QDialogButtonBox::Cancel)->setDisabled(false);
    DisablePanelIfNoSelection();
    if(index==-1)
    {
        //in the middle of a list clear. Running further will cause seg fault
        this->ui->listAlmBtn->button(QDialogButtonBox::Ignore)->setDisabled(true);//clone button
        return;
    }
    this->ui->listAlmBtn->button(QDialogButtonBox::Ignore)->setDisabled(false);
    ScheduleModel *active=this->_Schedules->GetSchedule(index);
    ui->timeEdit->setTime(active->AlarmTime);

    ui->chkCustom->setChecked(active->isCustomAlarmEnabled);
    ui->CustEdit->setDate(active->CustomAlarm);
    ui->chkSounds->setChecked(active->isCustomSoundEnabled);
    ui->txtSoundPath->setText(active->CustomSoundPath);

    ui->chkMon->setChecked(active->isMonEnabled);
    ui->chkTues->setChecked(active->isTueEnabled);
    ui->chkWed->setChecked(active->isWedEnabled);
    ui->chkThurs->setChecked(active->isThurEnabled);
    ui->chkFri->setChecked(active->isFriEnabled);
    ui->chkSat->setChecked(active->isSatEnabled);
    ui->chkSun->setChecked(active->isSunEnabled);
    ui->calendarWidget->setSelectedDate(active->CustomAlarm);

    ui->chkBastard->setChecked(active->isBastard);
    ui->chkOnshot->setChecked(active->isOneshot);

    if(active->isOneshot)
    {
        DisableGUIIfOneshot(true);
    }
}

void alarmMainWindow::DisableGUIIfOneshot(bool value)
{
    ui->chkFri->setDisabled(value);
    ui->chkThurs->setDisabled(value);
    ui->chkWed->setDisabled(value);
    ui->chkTues->setDisabled(value);
    ui->chkMon->setDisabled(value);
    ui->chkSun->setDisabled(value);
    ui->chkSat->setDisabled(value);
}

void alarmMainWindow::timeCheck()
{
//    qDebug() << "Time Check!";
    UpdateClock();
    SnoozeMenuCheck();
    BastardMenuCheck();
    if(_isMilTime!=_prevTimeWasMil)
    {
        _prevTimeWasMil=_isMilTime;
        displayTimeMode();
    }
}

void alarmMainWindow::UpdateClock()
{
    if(_isMilTime)
    {
//        ui->Clock->setText(QTime::currentTime().toString("H:mm:ss"));
        ui->Clock->setText(this->currentTime.toString("H:mm:ss"));
    }else{
//        ui->Clock->setText(QTime::currentTime().toString("H:mm:ss ap"));
        ui->Clock->setText(this->currentTime.toString("h:mm:ss ap"));
    }
}


void alarmMainWindow::OpenDiaglog(bool isChecked)
{
    ScheduleModel *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->isCustomSoundEnabled = isChecked;
    if(isChecked)
    {
        QString AlarmLocation = QFileDialog::getOpenFileName(this,"Select Alarm File",QDir::homePath());
        Active->CustomSoundPath = AlarmLocation;
        ui->txtSoundPath->setText(AlarmLocation);
    }else{
        ui->txtSoundPath->setText("");
    }
    this->_Schedules->Save();
}

void alarmMainWindow::TestAlarm()
{
    if(ui->chkSounds->isChecked())
    {
        this->CurAlarm->SetCustomPath(ui->txtSoundPath->text());
        this->CurAlarm->Start(true);
    }else{
        this->CurAlarm->Start(false);
    }
    CurAlarm->isBastard=this->ui->chkBastard->isChecked();
    this->testrun=true;
}

void alarmMainWindow::ShowAbout()
{
    AboutDialog *aboutbox=new AboutDialog(this);
    aboutbox->show();
}


void alarmMainWindow::SnoozeMenuCheck()
{
    if((this->testrun==true && this->CurAlarm->isBastard==false) || (this->CurAlarm->isPlaying() && this->CurAlarm->canResume && this->CurAlarm->isBastard==false))
    {
        //Create Snooze Menu object
        snooze *snMenu=new snooze(this,CurAlarm, this->_Schedules);
        snMenu->show();
        if(this->_supportsTray && this->testrun==false)
        {
            this->hide();
        }
        this->CurAlarm->canResume=false;
        this->testrun=false;
    }
}

void alarmMainWindow::BastardMenuCheck() // 定义 MainWindow 类的成员函数 BastardMenuCheck，用于检查和处理“Bastard”菜单的操作
{
    // 检查是否需要显示 BastardSnooze 对话框
    if((this->testrun == true && this->CurAlarm->isBastard == true) ||
       (this->CurAlarm->isPlaying() && this->CurAlarm->canResume && this->CurAlarm->isBastard))
    {
        // 如果是测试运行且当前警报是“Bastard”模式，或者警报正在播放且可以恢复并且是“Bastard”模式
        BastardSnooze *bsnooze = new BastardSnooze(this, this->CurAlarm, this->_Schedules); // 创建一个 BastardSnooze 对象

        qDebug() << "Let's do a math problem!";

        bsnooze->show(); // 显示 BastardSnooze 对话框

        if(this->_supportsTray && this->testrun == false) // 如果系统支持托盘并且不是测试运行
        {
            this->hide(); // 隐藏主窗口
        }

        this->CurAlarm->canResume = false; // 设置当前警报的 canResume 属性为 false，表示警报不能恢复
        this->testrun = false; // 设置 testrun 属性为 false，表示不再是测试运行
    }
}

void alarmMainWindow::PMWarning()
{
    QMessageBox::warning(this,"Time Verification","Your selected time is set for PM. If you intended to wake in the morning, now is your chance to fix it");
}


void alarmMainWindow::ShowSettings()
{
    SettingDialog *settingsPage=new SettingDialog(this,&_isMilTime, &_WarnOnPm, trayIcon);
    settingsPage->show();
}

void alarmMainWindow::displayTimeMode()
{
    if(_isMilTime)
    {
        ui->timeEdit->setDisplayFormat("H:mm:ss");
        ui->CustEdit->setDisplayFormat("d MMM yyyy");
    }else{
        ui->timeEdit->setDisplayFormat("h:mm:ss ap");
        ui->CustEdit->setDisplayFormat("d MMM yyyy");

    }
}

void alarmMainWindow::PopulateListWidget()
{
    ui->listWidget->clear();
    ScheduleModel *sche;
    foreach(sche,this->_Schedules->GetScheduleList())
    {
        ui->listWidget->addItem(sche->Name());
    }
    ui->listWidget->setCurrentRow(this->_lastDeletedIndex);
}

void alarmMainWindow::DisablePanelIfNoSelection()
{
    if(ui->listWidget->currentRow()==-1)
    {
        ui->chkCustom->setEnabled(false);
        ui->chkFri->setEnabled(false);
        ui->chkMon->setEnabled(false);
        ui->chkSat->setEnabled(false);
        ui->chkSounds->setEnabled(false);
        ui->chkSun->setEnabled(false);
        ui->chkThurs->setEnabled(false);
        ui->chkTues->setEnabled(false);
        ui->chkWed->setEnabled(false);
        ui->CustEdit->setEnabled(false);
        ui->timeEdit->setEnabled(false);
        ui->chkBastard->setEnabled(false);
        ui->lblTime->setEnabled(false);
        ui->chkOnshot->setEnabled(false);

        ui->chkCustom->setChecked(false);
        ui->chkFri->setChecked(false);
        ui->chkMon->setChecked(false);
        ui->chkBastard->setChecked(false);
        ui->chkSat->setChecked(false);
        ui->chkSounds->setChecked(false);
        ui->chkSun->setChecked(false);
        ui->chkThurs->setChecked(false);
        ui->chkTues->setChecked(false);
        ui->chkWed->setChecked(false);
        ui->txtSoundPath->setText("");
        ui->chkOnshot->setChecked(false);

    }
    else
    {
        ui->chkCustom->setEnabled(true);
        ui->chkFri->setEnabled(true);
        ui->chkMon->setEnabled(true);
        ui->chkSat->setEnabled(true);
        ui->chkSounds->setEnabled(true);
        ui->chkSun->setEnabled(true);
        ui->chkThurs->setEnabled(true);
        ui->chkTues->setEnabled(true);
        ui->chkWed->setEnabled(true);
        ui->CustEdit->setEnabled(true);
        ui->timeEdit->setEnabled(true);
        ui->chkBastard->setEnabled(true);
        ui->lblTime->setEnabled(true);
        ui->chkOnshot->setEnabled(true);
    }
}

void alarmMainWindow::UpdateListWidget()
{
    int index=ui->listWidget->currentRow();
    PopulateListWidget();
    ui->listWidget->setCurrentRow(index);
    this->_Schedules->Save();
}

void alarmMainWindow::SendTrayMessage(QString title, QString message)
{
    if(!FileIO::LoadSeenSolveText())
    {
        QIcon icon;
        if(FileIO::LoadisMono())
        {
            icon = QIcon(":/new/icons/Icons/clockSmall.png");
        }else{
            icon = QIcon(":/new/icons/Icons/clockSmall.png");
        }
        trayIcon->showMessage(title,message,icon);
        FileIO::SaveSeenSolveText();
    }
}

void alarmMainWindow::ChangeIconToDefault()
{
    trayIcon->setToolTip("Clock");
    if(FileIO::LoadisMono())
    {
        this->trayIcon->setIcon(QIcon(":/new/icons/Icons/clockSmall.png"));
    }else{
        this->trayIcon->setIcon(QIcon(":/new/icons/Icons/clockSmall.png"));
    }
}


void alarmMainWindow::SetupSlots(QAction *QAquit, QAction *QAshow)
{
    // 设置 UI 槽函数（信号与槽的连接）

    connect(QAquit, SIGNAL(triggered()), this, SLOT(Quit()));
    // 连接“退出”动作（QAquit）的触发信号到 MainWindow 的 Quit 槽函数

    connect(QAshow, SIGNAL(triggered()), this, SLOT(ToggleWindow()));
    // 连接“显示”动作（QAshow）的触发信号到 MainWindow 的 ToggleWindow 槽函数

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(ToggleWindow(QSystemTrayIcon::ActivationReason)));
    // 连接托盘图标的激活信号（例如左键单击、右键单击）到 MainWindow 的 ToggleWindow 槽函数，并传递激活的原因

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(Quit()));
    // 连接 UI 中的“退出”动作的触发信号到 MainWindow 的 Quit 槽函数

    connect(ui->actionAbout_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    // 连接 UI 中的“关于 QT”动作的触发信号到应用程序的 aboutQt 槽函数，显示关于 Qt 的信息

    connect(ui->actionAbout_QTalam, SIGNAL(triggered()), this, SLOT(ShowAbout()));
    // 连接 UI 中的“关于 QTalam”动作的触发信号到 MainWindow 的 ShowAbout 槽函数，显示应用程序的关于信息

    connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(ShowSettings()));
    // 连接 UI 中的“设置”动作的触发信号到 MainWindow 的 ShowSettings 槽函数，显示设置界面

    connect(ui->timeEdit, SIGNAL(editingFinished()), this, SLOT(SetTime()));
    // 连接时间编辑框的编辑完成信号到 MainWindow 的 SetTime 槽函数，用于设置时间

    connect(ui->listAlmBtn, SIGNAL(clicked(QAbstractButton*)), this, SLOT(AddRemoveAlarm(QAbstractButton*)));
    // 连接警报列表按钮的点击信号到 MainWindow 的 AddRemoveAlarm 槽函数，用于添加或删除警报

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(ShowActiveAlarm(int)));
    // 连接列表控件的当前行变化信号到 MainWindow 的 ShowActiveAlarm 槽函数，用于显示当前选中的警报

    connect(ui->chkMon, SIGNAL(clicked(bool)), this, SLOT(ToggleMon(bool)));
    // 连接星期一复选框的点击信号到 MainWindow 的 ToggleMon 槽函数，切换星期一的状态

    connect(ui->chkTues, SIGNAL(clicked(bool)), this, SLOT(ToggleTue(bool)));
    // 连接星期二复选框的点击信号到 MainWindow 的 ToggleTue 槽函数，切换星期二的状态

    connect(ui->chkWed, SIGNAL(clicked(bool)), this, SLOT(ToggleWed(bool)));
    // 连接星期三复选框的点击信号到 MainWindow 的 ToggleWed 槽函数，切换星期三的状态

    connect(ui->chkThurs, SIGNAL(clicked(bool)), this, SLOT(ToggleThur(bool)));
    // 连接星期四复选框的点击信号到 MainWindow 的 ToggleThur 槽函数，切换星期四的状态

    connect(ui->chkFri, SIGNAL(clicked(bool)), this, SLOT(ToggleFri(bool)));
    // 连接星期五复选框的点击信号到 MainWindow 的 ToggleFri 槽函数，切换星期五的状态

    connect(ui->chkSat, SIGNAL(clicked(bool)), this, SLOT(ToggleSat(bool)));
    // 连接星期六复选框的点击信号到 MainWindow 的 ToggleSat 槽函数，切换星期六的状态

    connect(ui->chkSun, SIGNAL(clicked(bool)), this, SLOT(ToggleSun(bool)));
    // 连接星期日复选框的点击信号到 MainWindow 的 ToggleSun 槽函数，切换星期日的状态

    connect(ui->chkCustom, SIGNAL(clicked(bool)), this, SLOT(ToggleCust(bool)));
    // 连接自定义复选框的点击信号到 MainWindow 的 ToggleCust 槽函数，切换自定义警报状态

    connect(ui->chkSounds, SIGNAL(clicked(bool)), this, SLOT(OpenDiaglog(bool)));
    // 连接声音复选框的点击信号到 MainWindow 的 OpenDiaglog 槽函数，打开一个对话框

    connect(ui->chkBastard, SIGNAL(clicked(bool)), this, SLOT(ToggleBastard(bool)));
    // 连接一个复选框（假设为“Bastard”）的点击信号到 MainWindow 的 ToggleBastard 槽函数，切换状态

    connect(ui->TestBtn, SIGNAL(clicked()), this, SLOT(TestAlarm()));
    // 连接测试按钮的点击信号到 MainWindow 的 TestAlarm 槽函数，用于测试警报

    connect(ui->VolumeSlider, SIGNAL(valueChanged(int)), CurAlarm, SLOT(SetVolume(int)));
    // 连接音量滑块的值变化信号到当前警报对象的 SetVolume 槽函数，用于设置警报音量

    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(SetCustomDate()));
    // 连接日历控件的点击信号到 MainWindow 的 SetCustomDate 槽函数，用于设置自定义日期

    connect(ui->chkOnshot, SIGNAL(clicked(bool)), this, SLOT(ToggleOneshot(bool)));
    // 连接一次性警报复选框的点击信号到 MainWindow 的 ToggleOneshot 槽函数，切换一次性警报的状态
}

void alarmMainWindow::SetupTrayIcon(QAction *QAshow, QAction *QAquit)
{
    trayIcon=new QSystemTrayIcon(this);
    trayIconMenu=new QMenu(this);

    trayIconMenu->addAction(QAshow);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(QAquit);
    trayIcon->setContextMenu(trayIconMenu);
    ChangeIconToDefault();
    trayIcon->show();

}

void alarmMainWindow::SetupVolume()
{
    int Volume = FileIO::LoadVolume();
    ui->VolumeSlider->setValue(Volume<=0? 50:Volume);
    CurAlarm->SetVolume(ui->VolumeSlider->value());
}

void alarmMainWindow::ConfigureButtons()
{
    this->ui->listAlmBtn->button(QDialogButtonBox::Ignore)->setText("Clone");
    this->ui->listAlmBtn->button(QDialogButtonBox::Ignore)->setDisabled(true);
    ui->listAlmBtn->button(QDialogButtonBox::Ok)->setText("&Add");
    ui->listAlmBtn->button(QDialogButtonBox::Cancel)->setText("&Remove");
    ui->listAlmBtn->button(QDialogButtonBox::Cancel)->setDisabled(true);
}

void alarmMainWindow::LoadOptions()
{
    _isMilTime=FileIO::isMilTime();
    _WarnOnPm=FileIO::LoadWarnOnPm();
    _prevTimeWasMil=_isMilTime;
    displayTimeMode();
}

void alarmMainWindow::TerminateIfRunning()
{
    const QString appKey = "qtalarmLocal";
    // 定义一个常量字符串 appKey，作为本地服务器的标识符

    QLocalSocket *socket = new QLocalSocket(this);
    // 创建一个 QLocalSocket 对象，用于检查当前是否已有实例在运行

    socket->connectToServer(appKey);
    // 连接到名为 appKey 的本地服务器，检查该服务器是否已经存在

    QLocalSocket::LocalSocketState status = socket->state();
    // 获取 socket 的当前状态

    if (status == QLocalSocket::ConnectedState)
    {
        socket->close();
        socket->deleteLater();
        // 如果 socket 连接成功，表示已经有一个应用实例在运行，关闭 socket 并释放资源

        qInfo() << "应用程序已经在运行";
        // 输出信息，通知用户应用程序已经在运行

        exit(EXIT_FAILURE);
        // 退出当前应用程序实例
    }
    socket->deleteLater();
    // 如果没有连接成功，释放 socket 资源

    // 用于进程间通信以检测重复的进程
    QLocalServer *m_server = new QLocalServer(this);
    // 创建一个 QLocalServer 对象，用于监听本地连接

    connect(m_server, SIGNAL(newConnection()), this, SLOT(handleLocalConnection()));
    // 连接新连接信号到 MainWindow 的 handleLocalConnection 槽函数，以便处理新连接

    m_server->setSocketOptions(QLocalServer::UserAccessOption);
    // 设置服务器的套接字选项，允许用户访问

    if (!m_server->listen(appKey))
    {
        qInfo() << "无法启动本地进程服务器";
        // 如果服务器监听失败，输出错误信息
    } else {
        qInfo() << "本地进程服务器已启动并正在运行";
        // 如果服务器成功监听，输出信息
    }
}

void alarmMainWindow::handleLocalConnection()
{
    if (toggleOnDuplicateProcess)
    {
        ToggleWindow();
        // 如果 toggleOnDuplicateProcess 为 true，则调用 ToggleWindow() 方法切换窗口的显示状态（显示或隐藏）。
    }
    else
    {
        this->show();
        // 如果 toggleOnDuplicateProcess 为 false，则显示主窗口。
    }
}



