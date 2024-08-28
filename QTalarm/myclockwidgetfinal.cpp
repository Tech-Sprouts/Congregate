#include "myclockwidgetfinal.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QNetworkAccessManager>

myClockWidgetFinal::myClockWidgetFinal(QWidget *parent)
    : QWidget(parent) {
    // 缩小比例
    float scale = 0.5;

    // 加载并缩小图片
    bg = QPixmap(":/ClockWidget_bg.png").scaled(
        QPixmap(":/ClockWidget_bg.png").size() * scale,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    hourHand = QPixmap(":/ClockWidget_hour.png").scaled(
        QPixmap(":/ClockWidget_hour.png").size() * 0.5 * scale,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    minuteHand = QPixmap(":/ClockWidget_minute.png").scaled(
        QPixmap(":/ClockWidget_minute.png").size() * 0.3 * scale,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    secondHand = QPixmap(":/ClockWidget_second.png").scaled(
        QPixmap(":/ClockWidget_second.png").size() * 0.3 * scale,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    centerImage = QPixmap(":/ClockWidget_center.png").scaled(
        QPixmap(":/ClockWidget_center.png").size() * 0.3 * scale,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    // 设置窗口大小与背景图片相同
    setFixedSize(bg.size());

    // 定时器每秒更新一次时钟
//    QTimer *timer = new QTimer(this);

//    timer->start(1000);

    // 创建网络管理器，用于处理HTTP请求
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    // 设置定时器，每隔100毫秒请求一次时间
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        QNetworkRequest request(QUrl("http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp")); // 淘宝API调用时间
        manager->get(request); // 发送GET请求
    });

        connect(timer, &QTimer::timeout, this, QOverload<>::of(&myClockWidgetFinal::update));

    timer->start(100); // 每500毫秒触发一次timeout信号
}

void myClockWidgetFinal::paintEvent(QPaintEvent *) {
    QTime currentTime = QTime::currentTime();

    int hour = currentTime.hour() % 12;
    int minute = currentTime.minute();
    int second = currentTime.second();

    // 计算时针、分针和秒针的旋转角度
    float hourAngle = (hour + minute / 60.0) * 30.0; // 360/12 = 30
    float minuteAngle = (minute + second / 60.0) * 6.0; // 360/60 = 6
    float secondAngle = second * 6.0; // 360/60 = 6

    QPainter painter(this);

    // 绘制背景
    // 设置不透明度为 50%
    painter.setOpacity(0.7);
    painter.drawPixmap(0, 0, bg);
    // 设置不透明度为 100%
    painter.setOpacity(1);

    // 中心点
    QPoint center(width() / 2, height() / 2);
    int clockRadius = qMin(width(), height()) / 2 - 20; // 时钟半径

    // 绘制刻度和数字
    painter.save();
    painter.setPen(QPen(Qt::white, 2));
    QFont font = painter.font();
    font.setPointSize(14);  // 缩小字体大小
    painter.setFont(font);

    for (int i = 0; i < 60; ++i) {
        int tickLength = (i % 5 == 0) ? 15 : 7; // 每5个刻度为长刻度
        painter.drawLine(center.x() + (clockRadius - tickLength) * qCos(qDegreesToRadians(i * 6.0 - 90)),
                         center.y() + (clockRadius - tickLength) * qSin(qDegreesToRadians(i * 6.0 - 90)),
                         center.x() + clockRadius * qCos(qDegreesToRadians(i * 6.0 - 90)),
                         center.y() + clockRadius * qSin(qDegreesToRadians(i * 6.0 - 90)));

        if (i % 5 == 0) { // 绘制数字
            QString number = QString::number(i / 5 == 0 ? 12 : i / 5);

            // 获取数字的大小
            QRect textRect = painter.boundingRect(QRect(), Qt::AlignCenter, number);

            // 调整文本的绘制位置，使得文本的中心与刻度对齐
            int textX = center.x() + (clockRadius - 30) * qCos(qDegreesToRadians(i * 6.0 - 90)) - textRect.width() / 2;
            int textY = center.y() + (clockRadius - 30) * qSin(qDegreesToRadians(i * 6.0 - 90)) - textRect.height() / 2;

            painter.drawText(textX, textY, textRect.width(), textRect.height(), Qt::AlignCenter, number);
        }
    }
    painter.restore();

    // 设置指针移动的半径
    int hourRadius = clockRadius - 100;   // 时针的半径
    int minuteRadius = clockRadius - 60; // 分针的半径
    int secondRadius = clockRadius - 20; // 秒针的半径

    // 计算各指针的位置
    QPoint hourPosition(center.x() + hourRadius * qCos(qDegreesToRadians(hourAngle - 90)),
                        center.y() + hourRadius * qSin(qDegreesToRadians(hourAngle - 90)));
    QPoint minutePosition(center.x() + minuteRadius * qCos(qDegreesToRadians(minuteAngle - 90)),
                          center.y() + minuteRadius * qSin(qDegreesToRadians(minuteAngle - 90)));
    QPoint secondPosition(center.x() + secondRadius * qCos(qDegreesToRadians(secondAngle - 90)),
                          center.y() + secondRadius * qSin(qDegreesToRadians(secondAngle - 90)));

    // 绘制时针从中心延伸出的线条
    painter.save();
    painter.setPen(QPen(Qt::white, 6 * 0.7)); // 缩小线条宽度
    painter.drawLine(center, hourPosition); // 绘制线段
    painter.restore();

    // 绘制分针从中心延伸出的线条
    painter.save();
    painter.setPen(QPen(Qt::white, 4 * 0.7)); // 缩小线条宽度
    painter.drawLine(center, minutePosition); // 绘制线段
    painter.restore();

    // 绘制秒针从中心延伸出的线条
    painter.save();
    painter.setPen(QPen(Qt::white, 2 * 0.7)); // 缩小线条宽度
    painter.drawLine(center, secondPosition); // 绘制线段
    painter.restore();

    // 绘制时针图片
    painter.save();
    painter.translate(hourPosition);
    painter.rotate(hourAngle);
    painter.drawPixmap(-hourHand.width() / 2, -hourHand.height() / 2, hourHand);
    painter.restore();

    // 绘制分针图片
    painter.save();
    painter.translate(minutePosition);
    painter.rotate(minuteAngle);
    painter.drawPixmap(-minuteHand.width() / 2, -minuteHand.height() / 2, minuteHand);
    painter.restore();

    // 绘制秒针图片
    painter.save();
    painter.translate(secondPosition);
    painter.rotate(secondAngle);
    painter.drawPixmap(-secondHand.width() / 2, -secondHand.height() / 2, secondHand);
    painter.restore();

    // 绘制中心图像
    painter.drawPixmap(center.x() - centerImage.width() / 2, center.y() - centerImage.height() / 2, centerImage);
}
