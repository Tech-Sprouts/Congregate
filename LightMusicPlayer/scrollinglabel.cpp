#include "scrollinglabel.h"
#include <QPainter>
#include <QResizeEvent>

ScrollingLabel::ScrollingLabel(QWidget *parent)
    : QLabel(parent),
      scrollPosition(0),
      scrollingSpeed(50)  // Default speed
{
    scrollTimer = new QTimer(this);
    connect(scrollTimer, &QTimer::timeout, this, &ScrollingLabel::startScrolling);
    scrollTimer->setInterval(scrollingSpeed);
}

void ScrollingLabel::setText(const QString &text)
{
    QLabel::setText(text);  // Call base class implementation
    currentText = text;
    scrollPosition = 0;
    startScrolling();
}

void ScrollingLabel::setScrollingSpeed(int speed)
{
    scrollingSpeed = speed;
    scrollTimer->setInterval(scrollingSpeed);
}

void ScrollingLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    startScrolling();  // Restart scrolling if size changes
}

void ScrollingLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if (currentText.isEmpty()) return;

    QPainter painter(this);
    painter.drawText(rect().adjusted(-scrollPosition, 0, 0, 0), Qt::TextSingleLine, currentText);
}

void ScrollingLabel::startScrolling()
{
    if (currentText.isEmpty()) return;

    int textWidth = fontMetrics().width(currentText);
    int labelWidth = width();

    if (textWidth <= labelWidth) {
        scrollTimer->stop();
        scrollPosition = 0;
        update();
        return;
    }

    scrollPosition += 2;  // Scroll step
    if (scrollPosition > textWidth) {
        scrollPosition = -labelWidth;
    }
    update();
}
