#include "bastardsnooze.h"  // 包含自定义头文件 "bastardsnooze.h"
#include "schedules.h"      // 包含自定义头文件 "schedules.h"
#include "ui_bastardsnooze.h" // 包含由 Qt 用户界面设计器生成的 UI 文件
#include <QRandomGenerator> // 包含 QRandomGenerator 类的头文件，用于生成随机数
#include <QDateTime>        // 包含 QDateTime 类的头文件，用于获取当前时间
#include <QMessageBox>      // 包含 QMessageBox 类的头文件，用于显示消息框
#include <QCloseEvent>      // 包含 QCloseEvent 类的头文件，用于处理窗口关闭事件

// 构造函数，初始化 BastardSnooze 类的实例
BastardSnooze::BastardSnooze(QWidget *parent, Alarm * curAlarm, Schedules * schedule_list) :
    QMainWindow(parent), // 调用父类 QMainWindow 的构造函数
    ui(new Ui::BastardSnooze) // 初始化指向 UI 对象的指针
{
    ui->setupUi(this); // 设置 UI 界面，初始化用户界面元素

    QRandomGenerator *generator = new QRandomGenerator(); // 创建 QRandomGenerator 对象，用于生成随机数
    generator->seed(QDateTime::currentSecsSinceEpoch());  // 使用当前时间（秒）作为种子，初始化随机数生成器
    val1 = generator->bounded(13); // 生成一个 0 到 12 之间的随机数并赋值给 val1
    val2 = generator->bounded(13); // 生成一个 0 到 12 之间的随机数并赋值给 val2
    randOp = generator->bounded(3); // 生成一个 0 到 2 之间的随机数并赋值给 randOp（用于选择操作符）

    this->ui->num1->setProperty("value", val1); // 将 val1 设置为界面元素 num1 的属性值
    this->ui->num2->setProperty("value", val2); // 将 val2 设置为界面元素 num2 的属性值

    // 根据 randOp 的值设置运算符的文本
    switch(randOp)
    {
    case 0:
        this->ui->Operator->setText("+"); // 如果 randOp 为 0，设置运算符为 "+"
        break;
    case 1:
        this->ui->Operator->setText("-"); // 如果 randOp 为 1，设置运算符为 "-"
        break;
    case 2:
        this->ui->Operator->setText("X"); // 如果 randOp 为 2，设置运算符为 "X"（乘法）
        break;
    }

    this->_curAlarm = curAlarm; // 将传入的当前警报对象指针 curAlarm 赋值给 _curAlarm
    this->_schdule_list = schedule_list; // 将传入的计划列表指针 schedule_list 赋值给 _schdule_list

    // 连接按钮和回车键的信号到 checkMath() 槽函数
    connect(ui->okbtn, SIGNAL(clicked()), this, SLOT(checkMath())); // 连接 okbtn 的点击信号到 checkMath() 槽函数
    connect(ui->Ans, SIGNAL(returnPressed()), this, SLOT(checkMath())); // 连接 Ans 输入框的回车信号到 checkMath() 槽函数
}

// 检查用户输入的数学答案是否正确
void BastardSnooze::checkMath()
{
    bool retVal = false; // 初始化返回值 retVal 为 false

    // 根据随机生成的运算符 randOp 计算正确答案，并与用户输入的答案进行比较
    switch (randOp) {
    case 0:
        retVal = val1 + val2 == ui->Ans->text().toInt(); // 如果 randOp 为 0，检查加法答案是否正确
        break;
    case 1:
        retVal = val1 - val2 == ui->Ans->text().toInt(); // 如果 randOp 为 1，检查减法答案是否正确
        break;
    case 2:
        retVal = val1 * val2 == ui->Ans->text().toInt(); // 如果 randOp 为 2，检查乘法答案是否正确
        break;
    }

    if(retVal) // 如果答案正确
    {
        this->_curAlarm->Stop(); // 停止当前警报
        if(this->_curAlarm->isOneshot) // 如果当前警报是一次性警报
        {
            this->_schdule_list->removeScheduleByIndex(this->_curAlarm->listId); // 从计划列表中移除对应的警报计划
        }
        this->hide(); // 隐藏当前窗口
        this->deleteLater(); // 延迟删除当前窗口对象
    } else { // 如果答案错误
        QMessageBox::critical(this, "WRONG", "WRONG"); // 显示错误消息框，提示用户答案错误
    }
}

// 析构函数，销毁 BastardSnooze 类的实例
BastardSnooze::~BastardSnooze()
{
    delete ui; // 删除指向 UI 对象的指针，释放内存
}

// 处理窗口关闭事件
void BastardSnooze::closeEvent(QCloseEvent *event)
{
    event->ignore(); // 忽略关闭事件，阻止窗口关闭
    QMessageBox::warning(this, "Nope", "you didn't think it was going to be that easy did you?"); // 显示警告消息框，提示用户不能轻易关闭窗口
}
