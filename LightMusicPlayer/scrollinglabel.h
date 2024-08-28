#ifndef SCROLLINGLABEL_H
#define SCROLLINGLABEL_H

#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>

class ScrollingLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ScrollingLabel(QWidget *parent = nullptr);
    void setText(const QString &text);
    void setScrollingSpeed(int speed);  // Set scrolling speed

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void startScrolling();

    QString currentText;
    QTimer *scrollTimer;
    int scrollPosition;
    int scrollingSpeed;
};

#endif // SCROLLINGLABEL_H
