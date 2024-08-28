#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QWebEngineView>
#include <QStandardItemModel>
#include "mychannel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MapWidget; }
QT_END_NAMESPACE

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    MapWidget(QWidget *parent = nullptr);
    ~MapWidget();
public slots:
    void on_cityLocation_clicked();
    void on_pointLocation_clicked();
    void on_keyLocation_clicked();
private slots:
    void on_bntReturn_clicked();

private:
    Ui::MapWidget *ui;
    myChannel* _myChannel;
};
#endif // MAPWIDGET_H
