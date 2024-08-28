#ifndef __MYCHANNEL_H__
#define __MYCHANNEL_H__

#include <QWebChannel>
#include <QJsonObject>

class myChannel :public QObject
{
    Q_OBJECT
public:
    explicit myChannel(QObject* parent=nullptr);
    Q_INVOKABLE  void getlnglat(qreal x,qreal y); // Q_INVOKABLE 函数才能直接在js调用
signals:
    void signal_getlnglat(qreal x,qreal y);



};


#endif
