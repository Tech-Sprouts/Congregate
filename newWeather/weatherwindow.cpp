#include "weatherwindow.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPainter>
#include <QPainter>
#include <QPen>
#include <QDir>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QFontDatabase>


#include "weatherData.h"
#include "ui_weatherwindow.h"
#include "weatherTool.h"
#include "worldlistwea.h"

#define INCREMENT 5     //温度每升高或降低1度，y轴坐标的增量
#define POINT_RADIUS 3  //曲线秒点的大小
#define TEXT_OFFSET_X 9    //温度文本相对于x点的偏移
#define TEXT_OFFSET_Y 7    //温度文本相对于y点的偏移

WeatherWindow::WeatherWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::WeatherWindow) {
    ui->setupUi(this);

    //设置窗口属性
    setWindowFlag(Qt::FramelessWindowHint);  // 设置无边框
    setFixedSize(width(), height());         // 设置固定窗口大小

    //设置应用图标
    setWindowIcon(QIcon(":/res/WeaFor.png"));

    QString fontPath = ":/res/AiDianGanFengXingKai-2.ttf";  // 自定义字体文件的路径
    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            QFont customFont(fontFamilies.at(0), 15);  // 创建自定义字体对象
            setFont(customFont);
        } else {
            qDebug() << "Failed to load the font from path:" << fontPath;
        }
    }

    //构建右键菜单
    mExitMenu = new QMenu(this);
    mExitAct = new QAction();
    mExitAct->setText(tr("退出"));
    mExitAct->setIcon(QIcon(":/res/close.ico"));
    mExitMenu->addAction(mExitAct);
    //用lamba函数连接退出按钮，触发就退出该应用
    connect(mExitAct, &QAction::triggered, this, [=]() { qApp->exit(0); });

    //天气图标录入
    weaType();

    //网络请求
    mNetAccessManager = new QNetworkAccessManager(this);
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&WeatherWindow::onReplied);

    //直接在构造中请求天气数据
    //目前默认为北京，可更改
    getWeatherInfo("北京");

    //给标签添加事件过滤器
    ui->lblHighCurve->installEventFilter(this);
    ui->lblLowCurve->installEventFilter(this);

//    //选择大洲窗口
//    connect(ui->Jump,&QPushButton::clicked,this,[=](){
//        List->setGeometry(this->geometry());
//        List->show();
//        this->hide();   //隐藏当前窗口
//    });



    //连接传递城市参数信号和槽
    connect(List,SIGNAL(setData(QString)),this,SLOT(receiveData(QString)));
}

WeatherWindow::~WeatherWindow() { delete ui; }

//重写父类虚函数
//父类中的默认实现是忽略右键菜单时间，重写后就可以
void WeatherWindow::contextMenuEvent(QContextMenuEvent* event) {
    //弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    event->accept();
}

void WeatherWindow::mousePressEvent(QMouseEvent *event)
{
    mOffset = event->globalPos() - this->pos();
}

void WeatherWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mOffset);
}

void WeatherWindow::getWeatherInfo(QString location)
{
    QString locationId = WeatherTool::getLocationId(location);
    if(locationId.isEmpty()){
        QMessageBox::warning(this,"提示","请检查城市名是否输错，该页面只支持国内。",QMessageBox::Ok);
        return;
    }
    mToday.location = location;

    locationfetcher->fetchLocation();

//    QUrl url("http://t.weather.itboy.net/api/weather/city/" + locationId);
//    QString apikey = "0c0b059e613f4824b77d1e8204f4cb82";
    QString apikey = "4fe7765a1cf5464dba2d152f372ba3a9";
    QUrl url7dWeather("https://devapi.qweather.com/v7/weather/7d?location="+locationId+"&key="+apikey);
    mNetAccessManager->get(QNetworkRequest(url7dWeather));
    QUrl url5dAir("https://devapi.qweather.com/v7/air/5d?location="+locationId+"&key="+apikey);
    mNetAccessManager->get(QNetworkRequest(url5dAir));
    QUrl urlRealWeather("https://devapi.qweather.com/v7/weather/now?location="+locationId+"&key="+apikey);
    mNetAccessManager->get(QNetworkRequest(urlRealWeather));
    QUrl urlRealAir("https://devapi.qweather.com/v7/air/now?location="+locationId+"&key="+apikey);
    mNetAccessManager->get(QNetworkRequest(urlRealAir));
    QUrl urlIndices("https://devapi.qweather.com/v7/indices/1d?type=1,2&location="+locationId+"&key="+apikey);
    mNetAccessManager->get(QNetworkRequest(urlIndices));
    QUrl urlRain("https://devapi.qweather.com/v7/minutely/5m?location="+locationfetcher->getLongtitude()+","+locationfetcher->getLatitude()+"&key="+apikey);
    qDebug() << urlRain;
    mNetAccessManager->get(QNetworkRequest(urlRain));
}

void WeatherWindow::parse7dWeather(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&err);  // 检测json格式
    if(err.error != QJsonParseError::NoError){    // Json格式错误
        return;
    }

    QJsonObject rootObj = doc.object();

    //解析预报中的5天数据
    QJsonArray forecatArr = rootObj.value("daily").toArray();
    for(int i = 0;i < 5;i++){
        QJsonObject objForecast = forecatArr[i].toObject();
        //mDay[i].week = objForecast.value("week").toString();
        mDay[i].fxdate = objForecast.value("fxDate").toString();

        //天气类型
        mDay[i].textDay = objForecast.value("textDay").toString();
        mDay[i].textNight = objForecast.value("textNight").toString();

        mDay[i].tempMax = objForecast.value("tempMax").toString().toInt();
        mDay[i].tempMin = objForecast.value("tempMin").toString().toInt();

        // 月相
        mDay[i].moonPhase = objForecast.value("moonPhase").toString();
//        qDebug() << mDay[i].moonPhase;

        //风向风力
        mDay[i].windDirDay = objForecast.value("windDirDay").toString();
        mDay[i].windscaleDay = objForecast.value("windScaleDay").toString();
    }

    QJsonObject nowObj = forecatArr[0].toObject();
    mToday.moonPhase = nowObj.value("moonPhase").toString(); // 月相

    qDebug() << "parse7dWeather";
    //更新UI
    updateUI();

    //绘制最高最低温度曲线
    ui->lblHighCurve->update();
    ui->lblLowCurve->update();
}

void WeatherWindow::parse5dAir(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&err);  // 检测json格式
    if(err.error != QJsonParseError::NoError){    // Json格式错误
        return;
    }

    QJsonObject rootObj = doc.object();


    //解析预报中的5天数据
    QJsonArray forecatArr = rootObj.value("daily").toArray();
    for(int i = 0;i < 5;i++){
        QJsonObject objForecast = forecatArr[i].toObject();

        //空气质量指数 【Air】
        mDay[i].category = objForecast.value("category").toString();
    }

    qDebug() << "parse5dAir";
    updateUI();

    QJsonObject nowObj = forecatArr[0].toObject();
    mToday.category = nowObj.value("category").toString(); // 空气质量

    //绘制最高最低温度曲线
    ui->lblHighCurve->update();
    ui->lblLowCurve->update();
}

void WeatherWindow::parseRealTimeWeather(const QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);  // 检测 JSON 格式
    if (err.error != QJsonParseError::NoError) {  // JSON 格式错误
        return;
    }

    QJsonObject rootObj = doc.object();
    if (rootObj.value("code").toString() != "200") {
        // 检查返回的 code 是否为 200，表示请求成功
        QMessageBox::warning(this, "天气", "实时天气数据请求失败", QMessageBox::Ok);
        return;
    }

    QJsonObject nowObj = rootObj.value("now").toObject();

    // 更新 mToday 数据
    mToday.temperature = nowObj.value("temp").toString().toDouble();
    mToday.type = nowObj.value("text").toString(); // 天气描述，如 "多云"
    mToday.icon = nowObj.value("icon").toString();
    mToday.windDir = nowObj.value("windDir").toString(); // 风向
    mToday.windScale = nowObj.value("windScale").toString(); // 风力
    mToday.humidity = nowObj.value("humidity").toString() + "%"; // 湿度


    // 天气对应背景
    qDebug() << mToday.icon[0];
    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/res/bgType/bg_"+mToday.icon[0]+".png);}");

    qDebug() << "parseRealTimeWeather";
    // 更新 UI
    updateUI();
}

void WeatherWindow::parseRealTimeAir(const QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);  // 检测 JSON 格式
    if (err.error != QJsonParseError::NoError) {  // JSON 格式错误
        return;
    }

    QJsonObject rootObj = doc.object();
    if (rootObj.value("code").toString() != "200") {
        // 检查返回的 code 是否为 200，表示请求成功
        QMessageBox::warning(this, "空气", "实时空气数据请求失败", QMessageBox::Ok);
        return;
    }

    QJsonObject nowObj = rootObj.value("now").toObject();
    // 更新 mToday 数据
    mToday.pm25 = nowObj.value("pm2p5").toString().toDouble();

    qDebug() << "parseRealTimeAir";
    // 更新 UI
    updateUI();
}

void WeatherWindow::parseIndices(const QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);  // 检测 JSON 格式
    if (err.error != QJsonParseError::NoError) {  // JSON 格式错误
        return;
    }

    QJsonObject rootObj = doc.object();
    if (rootObj.value("code").toString() != "200") {
        // 检查返回的 code 是否为 200，表示请求成功
        QMessageBox::warning(this, "空气", "实时空气数据请求失败", QMessageBox::Ok);
        return;
    }

    QJsonArray forecatArr = rootObj.value("daily").toArray();
    for(int i = 0;i < 2;i++){
        QJsonObject objForecast = forecatArr[i].toObject();

        //运动指数
        if(i == 0){
            mToday.sport = objForecast.value("text").toString();
        }
        else if(i == 1){
            mToday.washCar = objForecast.value("text").toString();
        }

    }


    // 更新 UI
    updateUI();
}

void WeatherWindow::parseRain(const QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);  // 检测 JSON 格式
    if (err.error != QJsonParseError::NoError) {  // JSON 格式错误
        return;
    }

    QJsonObject rootObj = doc.object();
    if (rootObj.value("code").toString() != "200") {
        // 检查返回的 code 是否为 200，表示请求成功
        QMessageBox::warning(this, "天气", "实时天气数据请求失败", QMessageBox::Ok);
        return;
    }

    mToday.rainText = rootObj.value("summary").toString();


    qDebug() << "parseRealTimeWeather";
    // 更新 UI
    updateUI();
}

void WeatherWindow::updateUI()
{
    QString fontPath = ":/res/AiDianGanFengXingKai-2.ttf";  // 自定义字体文件的路径
    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            QFont customFont(fontFamilies.at(0), 20);  // 创建自定义字体对象
            // 使用customFont设置文本样式等
            if (ui->lblDate) {  // 检查ui->lblDate是否为有效指针
                ui->lblDate->setStyleSheet("");  // 清除可能存在的样式表
                ui->lblCity->setStyleSheet("");  // 清除可能存在的样式表
                ui->lblDate->setFont(customFont);
                customFont.setPointSize(50);
                ui->lblCity->setFont(customFont);

                // 设置 QLabel 的字体颜色
                QPalette palette = ui->lblDate->palette();
                palette.setColor(QPalette::WindowText, Qt::white);  // 设置字体颜色为蓝色
                ui->lblDate->setPalette(palette);

                qDebug() << "Succeeded in loading font:" << fontFamilies.at(0);
            } else {
                qDebug() << "ui->lblDate is not a valid pointer.";
            }
        } else {
            qDebug() << "No font families found in the loaded font.";
        }
    } else {
        qDebug() << "Failed to load the font from path:" << fontPath;
    }



    //更新日期和城市
    ui->lblDate->setText(QDateTime::fromString(mToday.fxdate,"yyyy-MM-dd").toString("yyyy/MM/dd")
                         + " " + mDay[1].moonPhase);

    ui->lblCity->setText(mToday.location);

    //更新今天
    ui->lblTypeIcon->setPixmap(mTypeMap[mToday.type]);
    ui->lblTemp->setText(QString::number(mToday.temperature) + "°");
    ui->lblType->setText(mToday.type);
    ui->lblLowHigh->setText(QString::number(mToday.low) + "~"
                            + QString::number(mToday.high) + "°C");

    ui->lblSport->setText(mToday.sport); // "运动指数："
    ui->lblGanMao->setText(mToday.washCar); // "洗车指数："
    ui->lblRain->setText("Tips: "+mToday.rainText); // 分钟降雨
    ui->lblWindFx->setText(mToday.windDir);
    ui->lblWindFl->setText(mToday.windScale);

    ui->lblPM25->setText(QString::number(mToday.pm25));

    ui->lblShiDu->setText(mToday.humidity);
    ui->lblQuality->setText(mToday.category);

    //更新五天的数据
    for(int i = 0;i < 5;i++){
        //更新日期和时间
//        mWeekList[i]->setText("星期" + mDay[i].week.right(1));
        QString mDayList[5] = {"一", "二", "三", "四", "五"};
        mWeekList[i]->setText(mDayList[i]+"天后");
        ui->lblWeek0->setText("今天");
        ui->lblWeek1->setText("明天");
        ui->lblWeek2->setText("后天");
        QStringList ymdList = mDay[i].fxdate.split("-");
        mDateList[i]->setText(ymdList[1] + "/" + ymdList[2]);

        //更新天气类型(白天)
        mDayTypeList[i]->setText(mDay[i].textDay);
        mDayTypeIconList[i]->setPixmap(mTypeMap[mDay[i].textDay]);

        //更新天气类型(夜晚)
        mNightTypeList[i]->setText(mDay[i].textNight);
        mNightTypeIconList[i]->setPixmap(mTypeMap[mDay[i].textNight]);

        //更新空气质量
        if(mDay[i].category == "优"){
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color: rgba(139,195,74,0.5);");
        }else if(mDay[i].category == "良"){
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color: rgba(255,170,0,0.5);");
        }else if(mDay[i].category == "轻度"){
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color: rgba(255,87,97,0.5);");
        }else if(mDay[i].category == "中度"){
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color: rgba(255,17,27,0.5);");
        }else if(mDay[i].category == "重度"){
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color: rgba(170,0,0,0.5);");
        }else{
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color: rgba(110,0,0,0.5);");
        }

        //修正了100判断成10的情况
        int sdIndex = mToday.humidity.indexOf("%");
        QString shiduRes = mToday.humidity.left(sdIndex); // 取出 "市" 前面的子串
//        qDebug() << shiduRes;

        if(shiduRes.toInt() > 70){
            QPixmap high("://res/humidity2.png");
            ui->lblhumidity->setPixmap(high);
        }
        else {
            QPixmap low("://res/humidity2.png");
            ui->lblhumidity->setPixmap(low);
        }

        //更新风力、风向
        mFxList[i]->setText(mDay[i].windDirDay);
        mFlList[i]->setText(mDay[i].windscaleDay);
        qDebug() << mDay[i].windscaleDay;

    }
    QString city = ui->lblCity->text();
    QString temp = ui->lblLowHigh->text();
    QString type = mTypeMap[mDay[1].textDay];
    emit sendTemp(city,temp,type);
}

//消息过滤，主要用于重绘子控件，过滤Paint事件
bool WeatherWindow::eventFilter(QObject *watched, QEvent *event)
{
    //绘制最高温度曲线
    if(watched == ui->lblHighCurve && event->type() == QEvent::Paint){
        paintHighCurve();
    }
    //绘制最低温度曲线
    if(watched == ui->lblLowCurve && event->type() == QEvent::Paint){
        paintLowCurve();
    }
    return QWidget::eventFilter(watched,event);
}

void WeatherWindow::paintHighCurve()
{
    QPainter painter(ui->lblHighCurve);

    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 获取x坐标
    int pointX[5] = {0};
    for (int i = 0; i < 5; i++) {
        pointX[i] = mWeekList[i]->pos().x() + mWeekList[i]->width() / 2;
    }

    int tmpSum = 0;
    int tmpAvg = 0;
    for (int i = 0; i < 5; i++) {
        tmpSum += mDay[i].tempMax;
    }
    tmpAvg = tmpSum / 5;

    // 获取y坐标
    int pointY[5] = {0};
    int yCenter = ui->lblHighCurve->height() / 2;
    for (int i = 0; i < 5; i++) {
        pointY[i] = yCenter - ((mDay[i].tempMax - tmpAvg) * INCREMENT);
    }

    // 绘制
    QPen pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(QColor(243, 129, 129));
    painter.setPen(pen);
//    painter.setBrush(QColor(243, 129, 129)); // 设置画刷内部填充的颜色

    // 画点、写文本
    for (int i = 0; i < 5; i++) {
        painter.drawEllipse(QPoint(pointX[i], pointY[i]), POINT_RADIUS, POINT_RADIUS);
        // 显示温度文本
        painter.drawText(pointX[i] - TEXT_OFFSET_X, pointY[i] - TEXT_OFFSET_Y,
                         QString::number(mDay[i].tempMax) + "°");
    }

    // 绘制贝塞尔曲线
    QPainterPath path(QPointF(pointX[0], pointY[0]));
    for (int i = 0; i < 4; i++) {
        // 控制点计算
        qreal cpx1 = pointX[i] + (pointX[i + 1] - pointX[i]) / 3;
        qreal cpy1 = pointY[i] + (pointY[i + 1] - pointY[i]) / 3;
        qreal cpx2 = pointX[i + 1] - (pointX[i + 1] - pointX[i]) / 3;
        qreal cpy2 = pointY[i + 1] - (pointY[i + 1] - pointY[i]) / 3;

        // 添加贝塞尔曲线段
        path.cubicTo(QPointF(cpx1, cpy1), QPointF(cpx2, cpy2), QPointF(pointX[i + 1], pointY[i + 1]));
    }

    // 绘制路径
    painter.drawPath(path);
}


void WeatherWindow::paintLowCurve()
{
    QPainter painter(ui->lblLowCurve);

    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);

    //获取x坐标
    int pointX[5] = {0};
    for(int i = 0;i < 5;i++){
        pointX[i] = mWeekList[i]->pos().x() + mWeekList[i]->width() / 2;
    }

    //获取y坐标
    int pointY[5] = {0};
    int yCenter = ui->lblLowCurve->height() / 2;

    //求平均气温
    int tmpSum = 0;
    int tmpAvg = 0;
    for(int i = 0;i < 5;i++){
        tmpSum += mDay[i].tempMin;
    }
    tmpAvg = tmpSum / 5;

    for(int i = 0;i < 5;i++){
        pointY[i] = yCenter - ((mDay[i].tempMin - tmpAvg) * INCREMENT);
    }

    //绘制
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(17, 153, 158));

    painter.setPen(pen);
//    painter.setBrush(QColor(17, 153, 158));    //设置画刷内部填充的颜色

    // 画点、写文本
    for (int i = 0; i < 5; i++) {
        painter.drawEllipse(QPoint(pointX[i], pointY[i]), POINT_RADIUS, POINT_RADIUS);
        // 显示温度文本
        painter.drawText(pointX[i] - TEXT_OFFSET_X, pointY[i] - TEXT_OFFSET_Y,
                         QString::number(mDay[i].tempMin) + "°");
    }

    // 绘制贝塞尔曲线
    QPainterPath path(QPointF(pointX[0], pointY[0]));
    for (int i = 0; i < 4; i++) {
        // 控制点计算
        qreal cpx1 = pointX[i] + (pointX[i + 1] - pointX[i]) / 3;
        qreal cpy1 = pointY[i] + (pointY[i + 1] - pointY[i]) / 3;
        qreal cpx2 = pointX[i + 1] - (pointX[i + 1] - pointX[i]) / 3;
        qreal cpy2 = pointY[i + 1] - (pointY[i + 1] - pointY[i]) / 3;

        // 添加贝塞尔曲线段
        path.cubicTo(QPointF(cpx1, cpy1), QPointF(cpx2, cpy2), QPointF(pointX[i + 1], pointY[i + 1]));
    }

    // 绘制路径
    painter.drawPath(path);
}

void WeatherWindow::weaType()
{
    QString fontPath = ":/res/AiDianGanFengXingKai-2.ttf";  // 自定义字体文件的路径
    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            QFont customFont(fontFamilies.at(0), 20);  // 创建自定义字体对象
            ui->lblDate->setFont(customFont);
        } else {
            qDebug() << "Failed to load the font from path:" << fontPath;
        }
    }


    //给搜索绑定回车键
    ui->btnSearch->setShortcut(Qt::Key_Enter);
    ui->btnSearch->setShortcut(Qt::Key_Return);
    //将控件添加到控件数组
    mWeekList << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3 << ui->lblWeek4;
    mDateList << ui->lblDate0 << ui->lblDate1 << ui->lblDate2 << ui->lblDate3 << ui->lblDate4;

    //显示天气类型（白天）
    mDayTypeList << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3 << ui->lblType4;
    mDayTypeIconList << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3 << ui->lblTypeIcon4;
    //显示天气类型（夜晚）
    mNightTypeList << ui->lblType0_2 << ui->lblType1_2 << ui->lblType2_2 << ui->lblType3_2 << ui->lblType4_2;
    mNightTypeIconList << ui->lblTypeIcon0_2 << ui->lblTypeIcon1_2 << ui->lblTypeIcon2_2 << ui->lblTypeIcon3_2 << ui->lblTypeIcon4_2;

    mAqiList << ui->lblQuality0 << ui->lblQuality1 << ui->lblQuality2 << ui->lblQuality3 << ui->lblQuality4;

    mFxList << ui->lblFx0 << ui->lblFx1 << ui->lblFx2 << ui->lblFx3 << ui->lblFx4;
    // 创建一个标签指针列表
        QList<QLabel*> lblFlList = { ui->lblFl1, ui->lblFl2, ui->lblFl3, ui->lblFl4, ui->lblFl0 };

        // 设置颜色
        QString color = "balck"; // 可以根据条件动态设置颜色

        for (QLabel* lblFl : lblFlList) {
            if (lblFl) {  // 确保指针有效
                lblFl->setStyleSheet("color: " + color + ";");
            }
        }
    mFlList << ui->lblFl0 << ui->lblFl1 << ui->lblFl2 << ui->lblFl3 << ui->lblFl4;
    //天气对应图标
    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu2.png");
    mTypeMap.insert("暴雨到暴雪",":/res/type/BaoYu2.png");
    mTypeMap.insert("大暴雨",":/res/type/BaoYu2.png");
    mTypeMap.insert("大暴雨到大暴雪",":/res/type/BaoYu2.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大到暴雨",":/res/type/BaoYu2.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/BaoYu2.png");
    mTypeMap.insert("冻雨",":/res/type/BaoYu2.png");
    mTypeMap.insert("多云",":/res/type/DuoYun2.png");
    mTypeMap.insert("浮尘",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/ZhenYu2.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing2.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/BaoYu2.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoYu2");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu2.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin2.png");
    mTypeMap.insert("雨",":/res/type/XiaoYu2");
    mTypeMap.insert("雨夹雪",":/res/type/XiaoYu2");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu2.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("中雨",":/res/type/XiaoYu2.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
}

//void WeatherWindow::onReplied(QNetworkReply *reply)
//{
//    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    if(reply->error() != QNetworkReply::NoError || statusCode != 200){
//        qDebug() << reply->errorString().toLatin1().data();
//        QMessageBox::warning(this,"天气","请求数据失败",QMessageBox::Ok);
//    }else{
//        QByteArray  byteArray = reply->readAll();
////        qDebug() << "读所有：" << byteArray.data();
//        parseJson(byteArray);
//    }
//    reply->deleteLater();
//}

void WeatherWindow::onReplied(QNetworkReply *reply)
{
    // 检查网络请求状态
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this, "天气", "请求数据失败", QMessageBox::Ok);
    } else {
        QByteArray byteArray = reply->readAll();

        // 判断请求来自哪一个 url
        if (reply->url().toString().contains("devapi.qweather.com/v7/weather/now")) {
            // 解析实时天气数据
            parseRealTimeWeather(byteArray);
        }
        else if (reply->url().toString().contains("devapi.qweather.com/v7/air/now")){
            // 解析实时空气数据
            parseRealTimeAir(byteArray);
        }
        else if (reply->url().toString().contains("devapi.qweather.com/v7/weather/7d?")) {
            // 解析7d天气数据
            parse7dWeather(byteArray);
        }
        else if(reply->url().toString().contains("devapi.qweather.com/v7/air/5d")){
            // 解析5d空气数据
            parse5dAir(byteArray);
        }
        else if(reply->url().toString().contains("devapi.qweather.com/v7/indices/1d?type=1,2")){
            // 解析实时运动、洗车指数
            parseIndices(byteArray);
        }
        else if(reply->url().toString().contains("devapi.qweather.com/v7/minutely/5m?location=")){
            // 解析分钟降雨数据
            parseRain(byteArray);
        }
    }
    reply->deleteLater();
}


void WeatherWindow::on_btnSearch_clicked()
{
    QString cityName = ui->leCity->text();
    if(cityName == ""){
        return;
    }
    getWeatherInfo(cityName);
}

//获取数据并且判断是否在国内
void WeatherWindow::receiveData(QString cityName)
{
    QString cityCode = WeatherTool::getLocationId(cityName);
    if(cityCode.isEmpty()){
        QMessageBox::information(List,"提示","请检查城市名是否输错，该页面只支持国内。",QMessageBox::Ok);
    }
    else{
        QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
        mNetAccessManager->get(QNetworkRequest(url));
        connect(this,SIGNAL(sendTemp(QString,QString,QString)),List,SLOT(getTemp(QString,QString,QString)));
        List->hide();
        this->setGeometry(List->geometry());       //跳转定位问题
        this->show();
    }
}
