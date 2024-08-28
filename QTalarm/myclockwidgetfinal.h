#ifndef MYCLOCKWIDGETFINAL_H
#define MYCLOCKWIDGETFINAL_H

#include <QWidget>
#include <QPixmap>

class myClockWidgetFinal : public QWidget {
    Q_OBJECT

public:
    explicit myClockWidgetFinal(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QPixmap bg;
    QPixmap hourHand;
    QPixmap minuteHand;
    QPixmap secondHand;
    QPixmap centerImage;
};

#endif // MYCLOCKWIDGETFINAL_H
