// RotatableLabel.h
#ifndef ROTATABLELABEL_H
#define ROTATABLELABEL_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QPainter>

class RotatableLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)

public:
    explicit RotatableLabel(QWidget *parent = nullptr);

    qreal rotation() const { return m_rotation; }
    void setRotation(qreal angle);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    qreal m_rotation;
};

#endif // ROTATABLELABEL_H
