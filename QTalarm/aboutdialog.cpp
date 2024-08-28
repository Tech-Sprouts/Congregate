#include "aboutdialog.h" // 包含自定义的头文件 "aboutdialog.h"
#include "ui_aboutdialog.h" // 包含由 Qt 用户界面设计器生成的 UI 文件
#include <QPixmap> // 包含 QPixmap 类的头文件，用于处理图像

// AboutDialog 构造函数，初始化基类 QDialog，并设置父对象
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog) // 初始化指向 UI 对象的指针
{
    ui->setupUi(this); // 设置 UI 界面，初始化用户界面元素

    // 设置图标信息
    QPixmap image; // 创建一个 QPixmap 对象来存储图像
    image.load(":/new/icons/Clock.png"); // 从资源文件加载图标
    ui->lblIcon->setPixmap(image); // 设置标签（lblIcon）的图标为加载的图像

    QString InfoText; // 创建一个 QString 对象用于存储信息文本
    InfoText.append("这是一个时钟小程序"); // 添加应用程序信息
    InfoText.append("\n"); // 添加换行符
    InfoText.append("\n"); // 添加换行符
    InfoText.append("BIT~~~~"); // 添加文本信息
    InfoText.append("\n"); // 添加换行符
    ui->lblInfo->setText(InfoText); // 将信息文本设置为标签（lblInfo）的文本

    ui->lbllink->setText("美好的一天"); // 将链接文本设置为标签（lbllink）的文本

    ui->lblVersion->setText("Version: " + this->version); // 设置版本信息到标签（lblVersion）
}

// AboutDialog 析构函数，销毁 ui 对象，释放内存
AboutDialog::~AboutDialog()
{
    delete ui; // 删除指向 UI 对象的指针，释放内存
}
