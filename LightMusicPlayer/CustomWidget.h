// CustomWidget.h
#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGraphicsBlurEffect>

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWidget(QWidget *parent = nullptr);
    void setBackground(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_backgroundPixmap;
    QGraphicsBlurEffect *m_blurEffect;
};

#endif // CUSTOMWIDGET_H
