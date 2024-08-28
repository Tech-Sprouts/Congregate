// CustomWidget.cpp
#include "CustomWidget.h"
#include <QPainter>
#include <QResizeEvent>

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget(parent), m_blurEffect(new QGraphicsBlurEffect(this))
{
    setGraphicsEffect(m_blurEffect);
}

void CustomWidget::setBackground(const QPixmap &pixmap)
{
    m_backgroundPixmap = pixmap;
    update(); // 触发重绘以显示新背景
}

void CustomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!m_backgroundPixmap.isNull())
    {
        painter.drawPixmap(0, 0, width(), height(), m_backgroundPixmap);
    }
    QWidget::paintEvent(event);
}
