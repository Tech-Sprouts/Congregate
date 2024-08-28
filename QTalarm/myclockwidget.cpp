#include "myclockwidget.h"

#include <QPainter>
#include <QTime>
#include <QtMath>

myClockWidget::myClockWidget(QWidget *parent) : QWidget(parent)
{
    m_time = new QTimer(this);

    connect(m_time,&QTimer::timeout,[=](){
        this->update();
    });

    m_time->start(1000);
}

myClockWidget::~myClockWidget()
{
    stopTime();

    if (m_time) {
        delete m_time;
        m_time = Q_NULLPTR;
    }
}

void myClockWidget::setRadius(int radius)
{
    if (m_radius != radius) {
        m_radius = radius;
        update();
    }
}

void myClockWidget::startTime()
{
    if (!m_time->isActive())
        m_time->start(1000);
}

void myClockWidget::stopTime()
{
    if (m_time->isActive())
        m_time->stop();
}

void myClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPointF pos = QPointF(width() / 2.0, height() / 2.0);
    painter.translate(pos);

    // 绘制表盘边缘
    QRadialGradient gradient(0, 0, m_radius+15);
    gradient.setColorAt(0.8, QColor("#E4E8C6"));
    gradient.setColorAt(1.0, QColor("#018C4F"));
//    painter.setPen(pen);
    painter.setBrush(gradient);
    painter.drawEllipse(QPoint(0, 0), m_radius+10, m_radius+10);


    // 绘制表盘背景
    gradient = QRadialGradient(0, 0, m_radius);
    gradient.setColorAt(1.0, QColor("#FEAC97"));
    gradient.setColorAt(0.8, QColor("#ED5B46"));
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(0, 0), m_radius, m_radius);

    // 绘制内环
//    QPen pen(QColor("#018C4F"), 15);
//    pen.setColor(QColor("#009E6C"));
//    pen.setWidth(2);
//    painter.setPen(pen);
//    painter.drawEllipse(QPoint(0, 0), m_radius - 5, m_radius - 5);

    drawScale(&painter);
    drawScaleNum(&painter);
    drawPointer(&painter);
}

void myClockWidget::drawScale(QPainter *painter)
{
    painter->save();

//    QPen pen(QColor("#802D40"));
    QPen pen(QColor("#018C4F"));
    pen.setWidthF(2);
    painter->setPen(pen);

    for (int i = 1; i <= 12; ++i) {
        painter->drawLine(QPoint(0, -(m_radius - 10)), QPoint(0, -(m_radius - 20)));
        painter->rotate(30);
    }

    pen.setWidthF(1);
    painter->setPen(pen);
    for (int i = 1; i <= 60; ++i) {
        painter->drawLine(QPoint(0, -(m_radius - 10)), QPoint(0, -(m_radius - 16)));
        painter->rotate(6);
    }

    painter->restore();
}

void myClockWidget::drawScaleNum(QPainter *painter)
{
    int startAngle = 30;
    painter->save();
    painter->setPen(QColor(0, 0, 0));

    for (int i = 1; i <= 12; ++i) {
        qreal x = (m_radius - 30) * qSin(M_PI * startAngle / 180);
        qreal y = -(m_radius - 30) * qCos(M_PI * startAngle / 180);
        qreal textWidth = fontMetrics().horizontalAdvance(QString::number(i));
        qreal textHeight = fontMetrics().height();
        int pointX = int(x - textWidth / 2);
        int pointY = int(y + textHeight / 4);
        painter->drawText(pointX, pointY, QString::number(i));
        startAngle += 30;
    }
    painter->restore();
}

void myClockWidget::drawPointer(QPainter *painter)
{
    QTime time = QTime::currentTime();
    int hour = time.hour();
    int minute = time.minute();
//    int second = time.second();
    if (hour > 12)
        hour = hour - 12;

    painter->save();
    painter->setPen(Qt::NoPen); // 取消描边
    painter->setBrush(QColor("#018C4F"));
    QPolygon hPolygon;
    hPolygon << QPoint(0, 0) << QPoint(5, -8) << QPoint(0, -(m_radius / 3 * 2)) << QPoint(-5, -8);
    int angle = hour * 30;
    int pAngle = minute / 2;
    painter->rotate(angle + pAngle);
    painter->drawConvexPolygon(hPolygon);
    painter->restore();

    painter->save();
    painter->setPen(Qt::NoPen); // 取消描边
    painter->setBrush(QColor("#018C4F"));
    QPolygon mPolygon;
    mPolygon << QPoint(0, 0) << QPoint(5, -8) << QPoint(0, -(m_radius / 4 * 3)) << QPoint(-5, -8);
    int mAngle = minute * 6;
    painter->rotate(mAngle);
    painter->drawConvexPolygon(mPolygon);
    painter->restore();

//    painter->save();
//    painter->setPen(Qt::NoPen); // 取消描边
//    painter->setBrush(QColor("#000000"));
//    QPolygon sPolygon;
//    sPolygon << QPoint(0, 0) << QPoint(5, -8) << QPoint(0, -(m_radius - 10)) << QPoint(-5, -8);
//    int sAngle = second * 6;
//    painter->rotate(sAngle);
//    painter->drawConvexPolygon(sPolygon);
//    painter->restore();

    // 绘制中心圆点
    painter->save();
    painter->setPen(Qt::NoPen); // 取消描边
    painter->setBrush(QColor("#802D40"));
    painter->drawEllipse(QPoint(0, 0), 5, 5);
    painter->restore();
}

