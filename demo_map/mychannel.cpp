#include "mychannel.h"
#include <QDebug>
myChannel::myChannel(QObject *parent)
    :QObject(parent)
{
}

void myChannel::getlnglat(qreal x,qreal y)
{
    qDebug()<< "getlnglat";
    emit signal_getlnglat(x,y);
}


