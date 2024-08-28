#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QtMath>

#include "../harry_browser/browsermodule.h"
#include "../demo_map/mapwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , waveOffset(0)  // 初始化波浪偏移量
{
    ui->setupUi(this);

    // 初始化渐变颜色
    gradientStartColor = QColor(100, 150, 255);
    gradientEndColor = QColor(150, 200, 255);

    // 设置按钮样式和效果
    applyButtonStyles();

    // 连接按钮信号到槽函数
    connect(ui->weatherButton, &QPushButton::clicked, this, &MainWindow::openWeatherModule);
    connect(ui->clockButton, &QPushButton::clicked, this, &MainWindow::openClockModule);
    connect(ui->chatButton, &QPushButton::clicked, this, &MainWindow::openChatModule);
    connect(ui->musicButton, &QPushButton::clicked, this, &MainWindow::openMusicModule);
    connect(ui->imageViewerButton, &QPushButton::clicked, this, &MainWindow::openImageViewerModule);
    connect(ui->videoPlayerButton, &QPushButton::clicked, this, &MainWindow::openVideoPlayerModule);
    connect(ui->browserButton, &QPushButton::clicked, this, &MainWindow::openBrowserModule);
    connect(ui->mapButton, &QPushButton::clicked, this, &MainWindow::openMapModule);

    // 创建一个定时器来更新动画效果
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCircles);
    timer->start(30);  // 每30毫秒更新一次

    // 创建一个定时器用于动态背景
    backgroundTimer = new QTimer(this);
    connect(backgroundTimer, &QTimer::timeout, this, &MainWindow::updateBackground);
    backgroundTimer->start(50);  // 每50毫秒更新一次
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::applyButtonStyles() {
    // 为每个按钮设置样式和效果
    QList<QPushButton*> buttons = { ui->weatherButton, ui->clockButton, ui->chatButton,
                                    ui->musicButton, ui->imageViewerButton, ui->videoPlayerButton,
                                    ui->browserButton, ui->mapButton };

    for (QPushButton* button : buttons) {
        addShadowEffect(button);  // 添加阴影效果
        setupButtonHoverEffects(button);  // 设置悬停效果
    }
}

void MainWindow::addShadowEffect(QPushButton *button) {
    // 创建阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(3, 3);
    shadowEffect->setColor(QColor(0, 0, 0, 80));  // 设置阴影颜色和透明度
    button->setGraphicsEffect(shadowEffect);
}

void MainWindow::setupButtonHoverEffects(QPushButton *button) {
    // 更新样式表，不使用 transform 和 transition
    button->setStyleSheet(R"(
        QPushButton {
            background-color: #ffffff;
            border: 2px solid #007BFF;
            border-radius: 10px;
            padding: 10px;
            font-size: 18px;
            color: #007BFF;
        }
        QPushButton:hover {
            background-color: #007BFF;
            color: #ffffff;
        }
        QPushButton:pressed {
            background-color: #0056b3;
            color: #ffffff;
        }
    )");
}


void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 绘制动态渐变背景
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height()));
    linearGrad.setColorAt(0, gradientStartColor);
    linearGrad.setColorAt(1, gradientEndColor);
    painter.fillRect(rect(), linearGrad);

    // 绘制柔和的波浪效果
    QPainterPath wavePath;
    wavePath.moveTo(0, height() / 2);
    for (int x = 0; x < width()+20; x += 20) {
        int y = height() / 2 + qSin((x + waveOffset) * 0.1) * 20;  // 控制波浪的幅度和频率
        wavePath.lineTo(x, y);
    }
    wavePath.lineTo(width(), height());
    wavePath.lineTo(0, height());
    wavePath.closeSubpath();

    QColor waveColor = QColor(255, 255, 255, 100);  // 柔和的白色波浪
    painter.setBrush(waveColor);
    painter.setPen(Qt::NoPen);
    painter.drawPath(wavePath);

    // 绘制所有圆圈
    for (const CircleAnimation &circle : circles) {
        QColor circleColor = circle.color;
        circleColor.setAlpha(circle.alpha);  // 设置透明度
        painter.setBrush(circleColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(circle.center, circle.radius, circle.radius);
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        CircleAnimation circle;
        circle.center = event->pos();
        circle.radius = 0;
        circle.color = getRandomColor();
        circle.alpha = 255;  // 初始完全不透明
        circle.isTrail = false;  // 非拖尾效果
        circles.append(circle);
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {  // 检查左键是否按下
        CircleAnimation trailCircle;
        trailCircle.center = event->pos();
        trailCircle.radius = 5;  // 拖尾圆圈的初始半径较小
        trailCircle.color = QColor(255, 255, 255, 150);  // 拖尾圆圈的颜色和透明度
        trailCircle.alpha = 150;
        trailCircle.isTrail = true;  // 标记为拖尾效果
        circles.append(trailCircle);
        update();  // 触发重绘
    }
}


QColor MainWindow::getRandomColor() {
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);
    return QColor(r, g, b);
}

void MainWindow::updateCircles() {
    bool updated = false;

    for (int i = 0; i < circles.size(); ++i) {
        if (circles[i].isTrail) {  // 拖尾效果的圆圈
            circles[i].radius += 2;  // 拖尾圆圈半径增大速度较慢
            circles[i].alpha -= 15;  // 拖尾圆圈透明度更快减少
        } else {  // 普通圆圈
            circles[i].radius += 5;
            circles[i].alpha -= 10;
        }

        if (circles[i].radius > 200 || circles[i].alpha <= 0) {
            circles.removeAt(i);
            --i;
        }
        updated = true;
    }

    if (updated) {
        update();  // 重新绘制窗口
    }
}

void MainWindow::updateBackground() {
    waveOffset += 1;  // 增加波浪偏移量，控制波浪移动效果
    if (waveOffset > width()) {
        waveOffset = 0;
    }

    // 动态改变渐变颜色（例如，周期性地改变色调）
    int hueShift = (waveOffset / 5) % 360;  // 根据波浪偏移量来变化色调
    gradientStartColor.setHsv(hueShift, 150, 255);
    gradientEndColor.setHsv((hueShift + 30) % 360, 150, 255);

    // 根据当前渐变颜色计算按钮颜色
    QColor buttonColor = gradientStartColor.lighter(150);  // 按钮颜色比渐变颜色稍微亮一些

    // 将新颜色应用到所有按钮
    QList<QPushButton*> buttons = { ui->weatherButton, ui->clockButton, ui->chatButton,
                                    ui->musicButton, ui->imageViewerButton, ui->videoPlayerButton,
                                    ui->browserButton, ui->mapButton };

    for (QPushButton* button : buttons) {
        button->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #007BFF; border-radius: 10px; padding: 10px; font-size: 18px; color: #007BFF; }"
                                      "QPushButton:hover { background-color: #007BFF; color: #ffffff; }"
                                      "QPushButton:pressed { background-color: #0056b3; color: #ffffff; }")
                                  .arg(buttonColor.name()));
    }

    update();  // 触发重绘
}

// void MainWindow::animateButtonHover(QPushButton *button, bool hover) {
//     // 创建动画对象，设置目标对象和属性
//     QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
//     animation->setDuration(200);  // 动画持续时间

//     // 获取按钮的初始位置和尺寸
//     QRect startRect = button->geometry();
//     QRect endRect = startRect;

//     // 如果鼠标悬停，放大按钮；否则恢复原始大小
//     if (hover) {
//         endRect.setWidth(startRect.width() * 1.1);  // 放大 10%
//         endRect.setHeight(startRect.height() * 1.1);
//         endRect.moveCenter(startRect.center());  // 保持中心不变
//     }

//     animation->setStartValue(startRect);
//     animation->setEndValue(endRect);

//     // 开始动画
//     animation->start(QAbstractAnimation::DeleteWhenStopped);
// }




// 槽函数示例
void MainWindow::openWeatherModule() {
    // TODO: Implement the function to open the Weather module
}

void MainWindow::openClockModule() {
    // TODO: Implement the function to open the Clock module
}

void MainWindow::openChatModule() {
    // TODO: Implement the function to open the Chat module
}

void MainWindow::openMusicModule() {
    // TODO: Implement the function to open the Music module
}

void MainWindow::openImageViewerModule() {
    // TODO: Implement the function to open the Image Viewer module
}

void MainWindow::openVideoPlayerModule() {
    // TODO: Implement the function to open the Video Player module
}

void MainWindow::openBrowserModule() {
    // TODO: Implement the function to open the Browser module
}

void MainWindow::openMapModule() {
    // TODO: Implement the function to open the Map module
}

void MainWindow::on_browserButton_clicked()
{
     browsermodule browserModule;
     browserModule.showBrowser(QUrl(QStringLiteral("https://www.baidu.com")));
}

void MainWindow::on_mapButton_clicked()
{
    MapWidget w;
    w.show();
}
