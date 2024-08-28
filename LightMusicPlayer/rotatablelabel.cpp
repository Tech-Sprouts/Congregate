// RotatableLabel.cpp
#include "rotatablelabel.h"

RotatableLabel::RotatableLabel(QWidget *parent)
    : QLabel(parent), m_rotation(0)
{
}

void RotatableLabel::setRotation(qreal angle)
{
    m_rotation = angle;
    update(); // 触发重绘
}

void RotatableLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(width() / 2, height() / 2);
    painter.rotate(m_rotation);
    painter.translate(-width() / 2, -height() / 2);
    painter.drawPixmap(0, 0, pixmap()->scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
