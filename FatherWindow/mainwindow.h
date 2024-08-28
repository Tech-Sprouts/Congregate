#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QList>
#include <QColor>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CircleAnimation {
public:
    QPoint center;
    int radius;
    QColor color;
    int alpha;  // 透明度
    bool isTrail;  // 新增：标识是否为拖尾效果
};


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent *event) override;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void openWeatherModule();
    void openClockModule();
    void openChatModule();
    void openMusicModule();
    void openImageViewerModule();
    void openVideoPlayerModule();
    void openBrowserModule();
    void openMapModule();

    void on_browserButton_clicked();

    void on_mapButton_clicked();

private:
    Ui::MainWindow *ui;

    QList<CircleAnimation> circles;  // 存储所有圆圈的列表
    QColor getRandomColor();  // 获取随机颜色
    void updateCircles();  // 更新所有圆圈动画
    void applyButtonStyles();  // 应用按钮样式
    void addShadowEffect(QPushButton *button);  // 新增：为按钮添加阴影效果
    void setupButtonHoverEffects(QPushButton *button);  // 新增：设置按钮的悬停效果
    // void animateButtonHover(QPushButton *button, bool hover);  // 新增函数：处理按钮悬停动画



    QTimer *backgroundTimer;  // 定时器用于更新背景
    int waveOffset;  // 波浪的偏移量
    QColor gradientStartColor;  // 渐变起始颜色
    QColor gradientEndColor;  // 渐变结束颜色

    void updateBackground();  // 更新背景函数
};

#endif // MAINWINDOW_H
