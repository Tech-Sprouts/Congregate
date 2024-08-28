/********************************************************************************
** Form generated from reading UI file 'basewea.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASIAWEA_H
#define UI_ASIAWEA_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_AsiaWea
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblTypeIcon;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblTemp;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QLabel *lblCity;
    QHBoxLayout *horizontalLayout;
    QLabel *lblType;
    QFrame *line;
    QLabel *lblLowHigh;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *goBackBtn;
    QPushButton *listBtn;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lblDate;
    QWidget *widget_10;
    QVBoxLayout *verticalLayout_9;
    QSpacerItem *verticalSpacer;
    QLabel *lblGanMao;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_12;
    QLabel *lblWindIcon;
    QVBoxLayout *verticalLayout_6;
    QLabel *lblWindFx;
    QLabel *lblWindFl;
    QHBoxLayout *horizontalLayout_14;
    QLabel *lblPm25Icon;
    QVBoxLayout *verticalLayout_7;
    QLabel *lblPM25Title;
    QLabel *lblPM25;
    QHBoxLayout *horizontalLayout_13;
    QLabel *lblShiDuIcon;
    QVBoxLayout *verticalLayout_3;
    QLabel *lblShiDuTitle;
    QLabel *lblShiDu;
    QHBoxLayout *horizontalLayout_15;
    QLabel *lblQualityIcon;
    QVBoxLayout *verticalLayout_8;
    QLabel *lblQualityTitle;
    QLabel *lblQuality;
    QWidget *widget_11;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_5;
    QGridLayout *gridLayout_2;
    QLabel *lblWeek0;
    QLabel *lblDate2;
    QLabel *lblDate1;
    QLabel *lblDate0;
    QLabel *lblWeek1;
    QLabel *lblWeek2;
    QLabel *lblWeek3;
    QLabel *lblWeek4;
    QLabel *lblDate4;
    QLabel *lblDate3;
    QWidget *widget_6;
    QGridLayout *gridLayout_3;
    QLabel *lblTypeIcon2;
    QLabel *lblTypeIcon0;
    QLabel *lblType4;
    QLabel *lblType3;
    QLabel *lblType0;
    QLabel *lblTypeIcon4;
    QLabel *lblTypeIcon1;
    QLabel *lblTypeIcon3;
    QLabel *lblType2;
    QLabel *lblType1;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_11;
    QLabel *lblQuality0;
    QLabel *lblQuality1;
    QLabel *lblQuality2;
    QLabel *lblQuality3;
    QLabel *lblQuality4;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_5;
    QLabel *lblHighCurve;
    QLabel *lblLowCurve;
    QWidget *widget_8;
    QGridLayout *gridLayout_4;
    QLabel *lblFx3;
    QLabel *lblFx1;
    QLabel *lblFx0;
    QLabel *lblFx4;
    QLabel *lblFl2;
    QLabel *lblFl1;
    QLabel *lblFl4;
    QLabel *lblFx2;
    QLabel *lblFl0;
    QLabel *lblFl3;

    void setupUi(QMainWindow *AsiaWea)
    {
        if (AsiaWea->objectName().isEmpty())
            AsiaWea->setObjectName(QString::fromUtf8("AsiaWea"));
        AsiaWea->resize(800, 450);
        centralwidget = new QWidget(AsiaWea);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 800, 500));
        widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
"	border-image: url(:/res/bg.jpg);\n"
"}\n"
"\n"
"QLabel {\n"
"	font: 25 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"	border-radius: 4px;\n"
"	background-color: rgba(60, 60, 60, 100);\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(10, 70, 341, 121));
        widget_3->setStyleSheet(QString::fromUtf8("background-color: rgba(157, 133, 255, 0);\n"
"border-radius: 15px"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(8);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lblTypeIcon = new QLabel(widget_3);
        lblTypeIcon->setObjectName(QString::fromUtf8("lblTypeIcon"));
        lblTypeIcon->setMinimumSize(QSize(110, 110));
        lblTypeIcon->setMaximumSize(QSize(110, 110));
        lblTypeIcon->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        lblTypeIcon->setPixmap(QPixmap(QString::fromUtf8(":/res/type/DuoYun.png")));
        lblTypeIcon->setScaledContents(true);
        lblTypeIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lblTypeIcon);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblTemp = new QLabel(widget_3);
        lblTemp->setObjectName(QString::fromUtf8("lblTemp"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(50);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        lblTemp->setFont(font);
        lblTemp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 50pt \"Arial\";\n"
"background-color: rgba(0, 255, 255, 0);"));
        lblTemp->setScaledContents(true);
        lblTemp->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lblTemp);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, -1, 40);
        lblCity = new QLabel(widget_3);
        lblCity->setObjectName(QString::fromUtf8("lblCity"));
        lblCity->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"background-color: rgba(255, 255, 255,0);"));
        lblCity->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(lblCity);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        lblType = new QLabel(widget_3);
        lblType->setObjectName(QString::fromUtf8("lblType"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblType->sizePolicy().hasHeightForWidth());
        lblType->setSizePolicy(sizePolicy);
        lblType->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblType->setScaledContents(true);
        lblType->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblType);

        line = new QFrame(widget_3);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 15));
        line->setMaximumSize(QSize(16777215, 15));
        line->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        lblLowHigh = new QLabel(widget_3);
        lblLowHigh->setObjectName(QString::fromUtf8("lblLowHigh"));
        lblLowHigh->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblLowHigh->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblLowHigh);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout_2);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(0, 0, 801, 49));
        widget_4->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 170, 255,0);"));
        horizontalLayout_4 = new QHBoxLayout(widget_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(15, -1, 20, -1);
        goBackBtn = new QPushButton(widget_4);
        goBackBtn->setObjectName(QString::fromUtf8("goBackBtn"));
        goBackBtn->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0,1);"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        goBackBtn->setIcon(icon);

        horizontalLayout_4->addWidget(goBackBtn);

        listBtn = new QPushButton(widget_4);
        listBtn->setObjectName(QString::fromUtf8("listBtn"));
        listBtn->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0,1);"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/list.png"), QSize(), QIcon::Normal, QIcon::Off);
        listBtn->setIcon(icon1);

        horizontalLayout_4->addWidget(listBtn);

        horizontalSpacer_3 = new QSpacerItem(309, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        lblDate = new QLabel(widget_4);
        lblDate->setObjectName(QString::fromUtf8("lblDate"));
        lblDate->setStyleSheet(QString::fromUtf8("font: 20pt \"Arial\";\n"
"background-color: rgba(255, 255, 255,0);"));
        lblDate->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(lblDate);

        widget_10 = new QWidget(widget);
        widget_10->setObjectName(QString::fromUtf8("widget_10"));
        widget_10->setGeometry(QRect(10, 190, 341, 241));
        verticalLayout_9 = new QVBoxLayout(widget_10);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);

        lblGanMao = new QLabel(widget_10);
        lblGanMao->setObjectName(QString::fromUtf8("lblGanMao"));
        lblGanMao->setStyleSheet(QString::fromUtf8("background-color: rgba(60, 60, 60, 0);\n"
"padding-left: 5px;\n"
"padding-right: 5px;"));
        lblGanMao->setWordWrap(true);

        verticalLayout_9->addWidget(lblGanMao);

        widget_2 = new QWidget(widget_10);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 182, 193);\n"
"border-radius: 15px"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(50);
        gridLayout->setVerticalSpacing(20);
        gridLayout->setContentsMargins(40, 25, 40, 25);
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(0);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        lblWindIcon = new QLabel(widget_2);
        lblWindIcon->setObjectName(QString::fromUtf8("lblWindIcon"));
        lblWindIcon->setMinimumSize(QSize(40, 40));
        lblWindIcon->setMaximumSize(QSize(40, 40));
        lblWindIcon->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        lblWindIcon->setPixmap(QPixmap(QString::fromUtf8(":/res/wind.png")));
        lblWindIcon->setScaledContents(true);
        lblWindIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(lblWindIcon);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        lblWindFx = new QLabel(widget_2);
        lblWindFx->setObjectName(QString::fromUtf8("lblWindFx"));
        lblWindFx->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblWindFx->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(lblWindFx);

        lblWindFl = new QLabel(widget_2);
        lblWindFl->setObjectName(QString::fromUtf8("lblWindFl"));
        lblWindFl->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblWindFl->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(lblWindFl);


        horizontalLayout_12->addLayout(verticalLayout_6);


        gridLayout->addLayout(horizontalLayout_12, 0, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(0);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        lblPm25Icon = new QLabel(widget_2);
        lblPm25Icon->setObjectName(QString::fromUtf8("lblPm25Icon"));
        lblPm25Icon->setMinimumSize(QSize(40, 40));
        lblPm25Icon->setMaximumSize(QSize(40, 40));
        lblPm25Icon->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        lblPm25Icon->setPixmap(QPixmap(QString::fromUtf8(":/res/pm25.png")));
        lblPm25Icon->setScaledContents(true);
        lblPm25Icon->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(lblPm25Icon);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        lblPM25Title = new QLabel(widget_2);
        lblPM25Title->setObjectName(QString::fromUtf8("lblPM25Title"));
        lblPM25Title->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblPM25Title->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lblPM25Title);

        lblPM25 = new QLabel(widget_2);
        lblPM25->setObjectName(QString::fromUtf8("lblPM25"));
        lblPM25->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblPM25->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lblPM25);


        horizontalLayout_14->addLayout(verticalLayout_7);


        gridLayout->addLayout(horizontalLayout_14, 0, 1, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(0);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        lblShiDuIcon = new QLabel(widget_2);
        lblShiDuIcon->setObjectName(QString::fromUtf8("lblShiDuIcon"));
        lblShiDuIcon->setMinimumSize(QSize(40, 40));
        lblShiDuIcon->setMaximumSize(QSize(40, 40));
        lblShiDuIcon->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        lblShiDuIcon->setPixmap(QPixmap(QString::fromUtf8(":/res/humidity.png")));
        lblShiDuIcon->setScaledContents(true);
        lblShiDuIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_13->addWidget(lblShiDuIcon);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lblShiDuTitle = new QLabel(widget_2);
        lblShiDuTitle->setObjectName(QString::fromUtf8("lblShiDuTitle"));
        lblShiDuTitle->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblShiDuTitle->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblShiDuTitle);

        lblShiDu = new QLabel(widget_2);
        lblShiDu->setObjectName(QString::fromUtf8("lblShiDu"));
        lblShiDu->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblShiDu->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblShiDu);


        horizontalLayout_13->addLayout(verticalLayout_3);


        gridLayout->addLayout(horizontalLayout_13, 1, 0, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(0);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        lblQualityIcon = new QLabel(widget_2);
        lblQualityIcon->setObjectName(QString::fromUtf8("lblQualityIcon"));
        lblQualityIcon->setMinimumSize(QSize(40, 40));
        lblQualityIcon->setMaximumSize(QSize(40, 40));
        lblQualityIcon->setAutoFillBackground(false);
        lblQualityIcon->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        lblQualityIcon->setPixmap(QPixmap(QString::fromUtf8(":/res/aqi.png")));
        lblQualityIcon->setScaledContents(true);
        lblQualityIcon->setAlignment(Qt::AlignCenter);
        lblQualityIcon->setWordWrap(false);
        lblQualityIcon->setIndent(-1);

        horizontalLayout_15->addWidget(lblQualityIcon);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        lblQualityTitle = new QLabel(widget_2);
        lblQualityTitle->setObjectName(QString::fromUtf8("lblQualityTitle"));
        lblQualityTitle->setAutoFillBackground(false);
        lblQualityTitle->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblQualityTitle->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lblQualityTitle);

        lblQuality = new QLabel(widget_2);
        lblQuality->setObjectName(QString::fromUtf8("lblQuality"));
        lblQuality->setAutoFillBackground(false);
        lblQuality->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lblQuality->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lblQuality);


        horizontalLayout_15->addLayout(verticalLayout_8);


        gridLayout->addLayout(horizontalLayout_15, 1, 1, 1, 1);


        verticalLayout_9->addWidget(widget_2);

        widget_11 = new QWidget(widget);
        widget_11->setObjectName(QString::fromUtf8("widget_11"));
        widget_11->setGeometry(QRect(360, 40, 451, 411));
        verticalLayout_4 = new QVBoxLayout(widget_11);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widget_5 = new QWidget(widget_11);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgba(0, 200, 200, 200);\n"
"	border-radius: 4px;\n"
"}"));
        gridLayout_2 = new QGridLayout(widget_5);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(6);
        gridLayout_2->setVerticalSpacing(0);
        lblWeek0 = new QLabel(widget_5);
        lblWeek0->setObjectName(QString::fromUtf8("lblWeek0"));
        lblWeek0->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblWeek0->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblWeek0, 0, 0, 1, 1);

        lblDate2 = new QLabel(widget_5);
        lblDate2->setObjectName(QString::fromUtf8("lblDate2"));
        lblDate2->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblDate2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblDate2, 1, 2, 1, 1);

        lblDate1 = new QLabel(widget_5);
        lblDate1->setObjectName(QString::fromUtf8("lblDate1"));
        lblDate1->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblDate1->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblDate1, 1, 1, 1, 1);

        lblDate0 = new QLabel(widget_5);
        lblDate0->setObjectName(QString::fromUtf8("lblDate0"));
        lblDate0->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblDate0->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblDate0, 1, 0, 1, 1);

        lblWeek1 = new QLabel(widget_5);
        lblWeek1->setObjectName(QString::fromUtf8("lblWeek1"));
        lblWeek1->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblWeek1->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblWeek1, 0, 1, 1, 1);

        lblWeek2 = new QLabel(widget_5);
        lblWeek2->setObjectName(QString::fromUtf8("lblWeek2"));
        lblWeek2->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblWeek2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblWeek2, 0, 2, 1, 1);

        lblWeek3 = new QLabel(widget_5);
        lblWeek3->setObjectName(QString::fromUtf8("lblWeek3"));
        lblWeek3->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblWeek3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblWeek3, 0, 3, 1, 1);

        lblWeek4 = new QLabel(widget_5);
        lblWeek4->setObjectName(QString::fromUtf8("lblWeek4"));
        lblWeek4->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblWeek4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblWeek4, 0, 4, 1, 1);

        lblDate4 = new QLabel(widget_5);
        lblDate4->setObjectName(QString::fromUtf8("lblDate4"));
        lblDate4->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblDate4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblDate4, 1, 4, 1, 1);

        lblDate3 = new QLabel(widget_5);
        lblDate3->setObjectName(QString::fromUtf8("lblDate3"));
        lblDate3->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblDate3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lblDate3, 1, 3, 1, 1);


        verticalLayout_4->addWidget(widget_5);

        widget_6 = new QWidget(widget_11);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        widget_6->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgba(60, 60, 60, 100);\n"
"	border-radius: 4px;\n"
"}"));
        gridLayout_3 = new QGridLayout(widget_6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setVerticalSpacing(0);
        gridLayout_3->setContentsMargins(-1, 5, -1, 5);
        lblTypeIcon2 = new QLabel(widget_6);
        lblTypeIcon2->setObjectName(QString::fromUtf8("lblTypeIcon2"));
        lblTypeIcon2->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblTypeIcon2->setPixmap(QPixmap(QString::fromUtf8(":/res/type/DuoYun.png")));
        lblTypeIcon2->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblTypeIcon2, 0, 2, 1, 1);

        lblTypeIcon0 = new QLabel(widget_6);
        lblTypeIcon0->setObjectName(QString::fromUtf8("lblTypeIcon0"));
        lblTypeIcon0->setMinimumSize(QSize(0, 0));
        lblTypeIcon0->setMaximumSize(QSize(1000, 1000));
        lblTypeIcon0->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblTypeIcon0->setPixmap(QPixmap(QString::fromUtf8(":/res/type/Qing.png")));
        lblTypeIcon0->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblTypeIcon0, 0, 0, 1, 1);

        lblType4 = new QLabel(widget_6);
        lblType4->setObjectName(QString::fromUtf8("lblType4"));
        lblType4->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;"));
        lblType4->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblType4, 1, 4, 1, 1);

        lblType3 = new QLabel(widget_6);
        lblType3->setObjectName(QString::fromUtf8("lblType3"));
        lblType3->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;"));
        lblType3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblType3, 1, 3, 1, 1);

        lblType0 = new QLabel(widget_6);
        lblType0->setObjectName(QString::fromUtf8("lblType0"));
        lblType0->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;"));
        lblType0->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblType0, 1, 0, 1, 1);

        lblTypeIcon4 = new QLabel(widget_6);
        lblTypeIcon4->setObjectName(QString::fromUtf8("lblTypeIcon4"));
        lblTypeIcon4->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblTypeIcon4->setPixmap(QPixmap(QString::fromUtf8(":/res/type/ZhongYu.png")));
        lblTypeIcon4->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblTypeIcon4, 0, 4, 1, 1);

        lblTypeIcon1 = new QLabel(widget_6);
        lblTypeIcon1->setObjectName(QString::fromUtf8("lblTypeIcon1"));
        lblTypeIcon1->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblTypeIcon1->setPixmap(QPixmap(QString::fromUtf8(":/res/type/XiaoYu.png")));
        lblTypeIcon1->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblTypeIcon1, 0, 1, 1, 1);

        lblTypeIcon3 = new QLabel(widget_6);
        lblTypeIcon3->setObjectName(QString::fromUtf8("lblTypeIcon3"));
        lblTypeIcon3->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblTypeIcon3->setPixmap(QPixmap(QString::fromUtf8(":/res/type/DuoYun.png")));
        lblTypeIcon3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblTypeIcon3, 0, 3, 1, 1);

        lblType2 = new QLabel(widget_6);
        lblType2->setObjectName(QString::fromUtf8("lblType2"));
        lblType2->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;"));
        lblType2->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblType2, 1, 2, 1, 1);

        lblType1 = new QLabel(widget_6);
        lblType1->setObjectName(QString::fromUtf8("lblType1"));
        lblType1->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;"));
        lblType1->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lblType1, 1, 1, 1, 1);


        verticalLayout_4->addWidget(widget_6);

        widget_9 = new QWidget(widget_11);
        widget_9->setObjectName(QString::fromUtf8("widget_9"));
        horizontalLayout_11 = new QHBoxLayout(widget_9);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(-1, 5, -1, 5);
        lblQuality0 = new QLabel(widget_9);
        lblQuality0->setObjectName(QString::fromUtf8("lblQuality0"));
        lblQuality0->setStyleSheet(QString::fromUtf8("background-color: rgb(121, 184, 0);\n"
"padding:8px;"));
        lblQuality0->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(lblQuality0);

        lblQuality1 = new QLabel(widget_9);
        lblQuality1->setObjectName(QString::fromUtf8("lblQuality1"));
        lblQuality1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 187, 23);"));
        lblQuality1->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(lblQuality1);

        lblQuality2 = new QLabel(widget_9);
        lblQuality2->setObjectName(QString::fromUtf8("lblQuality2"));
        lblQuality2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 187, 23);"));
        lblQuality2->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(lblQuality2);

        lblQuality3 = new QLabel(widget_9);
        lblQuality3->setObjectName(QString::fromUtf8("lblQuality3"));
        lblQuality3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 187, 23);"));
        lblQuality3->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(lblQuality3);

        lblQuality4 = new QLabel(widget_9);
        lblQuality4->setObjectName(QString::fromUtf8("lblQuality4"));
        lblQuality4->setStyleSheet(QString::fromUtf8("background-color: rgb(121, 184, 0);\n"
"padding:8px;"));
        lblQuality4->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(lblQuality4);


        verticalLayout_4->addWidget(widget_9);

        widget_7 = new QWidget(widget_11);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        verticalLayout_5 = new QVBoxLayout(widget_7);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 5, -1, 5);
        lblHighCurve = new QLabel(widget_7);
        lblHighCurve->setObjectName(QString::fromUtf8("lblHighCurve"));
        lblHighCurve->setMinimumSize(QSize(0, 80));
        lblHighCurve->setMaximumSize(QSize(10000, 10000));
        lblHighCurve->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblHighCurve->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lblHighCurve);

        lblLowCurve = new QLabel(widget_7);
        lblLowCurve->setObjectName(QString::fromUtf8("lblLowCurve"));
        lblLowCurve->setMinimumSize(QSize(0, 80));
        lblLowCurve->setMaximumSize(QSize(10000, 10000));
        lblLowCurve->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;"));
        lblLowCurve->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lblLowCurve);


        verticalLayout_4->addWidget(widget_7);

        widget_8 = new QWidget(widget_11);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        gridLayout_4 = new QGridLayout(widget_8);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setVerticalSpacing(0);
        lblFx3 = new QLabel(widget_8);
        lblFx3->setObjectName(QString::fromUtf8("lblFx3"));
        lblFx3->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblFx3->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFx3, 0, 3, 1, 1);

        lblFx1 = new QLabel(widget_8);
        lblFx1->setObjectName(QString::fromUtf8("lblFx1"));
        lblFx1->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblFx1->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFx1, 0, 1, 1, 1);

        lblFx0 = new QLabel(widget_8);
        lblFx0->setObjectName(QString::fromUtf8("lblFx0"));
        lblFx0->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblFx0->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFx0, 0, 0, 1, 1);

        lblFx4 = new QLabel(widget_8);
        lblFx4->setObjectName(QString::fromUtf8("lblFx4"));
        lblFx4->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblFx4->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFx4, 0, 4, 1, 1);

        lblFl2 = new QLabel(widget_8);
        lblFl2->setObjectName(QString::fromUtf8("lblFl2"));
        lblFl2->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblFl2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFl2, 1, 2, 1, 1);

        lblFl1 = new QLabel(widget_8);
        lblFl1->setObjectName(QString::fromUtf8("lblFl1"));
        lblFl1->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblFl1->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFl1, 1, 1, 1, 1);

        lblFl4 = new QLabel(widget_8);
        lblFl4->setObjectName(QString::fromUtf8("lblFl4"));
        lblFl4->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblFl4->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFl4, 1, 4, 1, 1);

        lblFx2 = new QLabel(widget_8);
        lblFx2->setObjectName(QString::fromUtf8("lblFx2"));
        lblFx2->setStyleSheet(QString::fromUtf8("border-bottom-left-radius: 0px;\n"
"border-bottom-right-radius: 0px;"));
        lblFx2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFx2, 0, 2, 1, 1);

        lblFl0 = new QLabel(widget_8);
        lblFl0->setObjectName(QString::fromUtf8("lblFl0"));
        lblFl0->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblFl0->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFl0, 1, 0, 1, 1);

        lblFl3 = new QLabel(widget_8);
        lblFl3->setObjectName(QString::fromUtf8("lblFl3"));
        lblFl3->setStyleSheet(QString::fromUtf8("border-top-left-radius: 0px;\n"
"border-top-right-radius: 0px;\n"
"color: rgb(240, 240, 240);"));
        lblFl3->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFl3, 1, 3, 1, 1);


        verticalLayout_4->addWidget(widget_8);

        AsiaWea->setCentralWidget(centralwidget);

        retranslateUi(AsiaWea);

        QMetaObject::connectSlotsByName(AsiaWea);
    } // setupUi

    void retranslateUi(QMainWindow *AsiaWea)
    {
        AsiaWea->setWindowTitle(QApplication::translate("AsiaWea", "MainWindow"));
        lblTypeIcon->setText(QString());
        lblTemp->setText(QApplication::translate("AsiaWea", "13\302\260"));
        lblCity->setText(QApplication::translate("AsiaWea", "\345\214\227\344\272\254"));
        lblType->setText(QApplication::translate("AsiaWea", "\346\231\264\350\275\254\345\244\232\344\272\221"));
        lblLowHigh->setText(QApplication::translate("AsiaWea", "9\302\260~15\302\260"));
        goBackBtn->setText(QString());
        listBtn->setText(QString());
        lblDate->setText(QApplication::translate("AsiaWea", "2023/05/13 \346\230\237\346\234\237\345\233\233"));
        lblGanMao->setText(QApplication::translate("AsiaWea", "\346\204\237\345\206\222\346\214\207\346\225\260\357\274\232\345\220\204\347\261\273\344\272\272\347\276\244\345\217\257\350\207\252\347\224\261\346\264\273\345\212\250"));
        lblWindIcon->setText(QString());
        lblWindFx->setText(QApplication::translate("AsiaWea", "\350\245\277\345\214\227\351\243\216"));
        lblWindFl->setText(QApplication::translate("AsiaWea", "3\347\272\247"));
        lblPm25Icon->setText(QString());
        lblPM25Title->setText(QApplication::translate("AsiaWea", "PM2.5"));
        lblPM25->setText(QApplication::translate("AsiaWea", "8"));
        lblShiDuIcon->setText(QString());
        lblShiDuTitle->setText(QApplication::translate("AsiaWea", "\346\271\277\345\272\246"));
        lblShiDu->setText(QApplication::translate("AsiaWea", "72%" ));
        lblQualityIcon->setText(QString());
        lblQualityTitle->setText(QApplication::translate("AsiaWea", "\347\251\272\346\260\224\350\264\250\351\207\217" ));
        lblQuality->setText(QApplication::translate("AsiaWea", "\344\274\230" ));
        lblWeek0->setText(QApplication::translate("AsiaWea", "\344\273\212\345\244\251" ));
        lblDate2->setText(QApplication::translate("AsiaWea", "01/03" ));
        lblDate1->setText(QApplication::translate("AsiaWea", "01/02" ));
        lblDate0->setText(QApplication::translate("AsiaWea", "01/01" ));
        lblWeek1->setText(QApplication::translate("AsiaWea", "\346\230\216\345\244\251" ));
        lblWeek2->setText(QApplication::translate("AsiaWea", "\345\220\216\345\244\251" ));
        lblWeek3->setText(QApplication::translate("AsiaWea", "\345\221\250\345\233\233" ));
        lblWeek4->setText(QApplication::translate("AsiaWea", "\345\221\250\344\272\224" ));
        lblDate4->setText(QApplication::translate("AsiaWea", "01/05" ));
        lblDate3->setText(QApplication::translate("AsiaWea", "01/04" ));
        lblTypeIcon2->setText(QString());
        lblTypeIcon0->setText(QString());
        lblType4->setText(QApplication::translate("AsiaWea", "\344\270\255\351\233\250" ));
        lblType3->setText(QApplication::translate("AsiaWea", "\345\244\232\344\272\221" ));
        lblType0->setText(QApplication::translate("AsiaWea", "\346\231\264" ));
        lblTypeIcon4->setText(QString());
        lblTypeIcon1->setText(QString());
        lblTypeIcon3->setText(QString());
        lblType2->setText(QApplication::translate("AsiaWea", "\345\244\232\344\272\221" ));
        lblType1->setText(QApplication::translate("AsiaWea", "\345\260\217\351\233\250" ));
        lblQuality0->setText(QApplication::translate("AsiaWea", "\344\274\230" ));
        lblQuality1->setText(QApplication::translate("AsiaWea", "\350\211\257" ));
        lblQuality2->setText(QApplication::translate("AsiaWea", "\350\211\257" ));
        lblQuality3->setText(QApplication::translate("AsiaWea", "\350\211\257" ));
        lblQuality4->setText(QApplication::translate("AsiaWea", "\344\274\230" ));
        lblHighCurve->setText(QString());
        lblLowCurve->setText(QString());
        lblFx3->setText(QApplication::translate("AsiaWea", "\350\245\277\345\214\227\351\243\216" ));
        lblFx1->setText(QApplication::translate("AsiaWea", "\344\270\234\345\214\227\351\243\216" ));
        lblFx0->setText(QApplication::translate("AsiaWea", "\344\270\234\351\243\216" ));
        lblFx4->setText(QApplication::translate("AsiaWea", "\344\270\234\345\215\227\351\243\216" ));
        lblFl2->setText(QApplication::translate("AsiaWea", "2\347\272\247" ));
        lblFl1->setText(QApplication::translate("AsiaWea", "3\347\272\247" ));
        lblFl4->setText(QApplication::translate("AsiaWea", "2\347\272\247" ));
        lblFx2->setText(QApplication::translate("AsiaWea", "\350\245\277\345\214\227\351\243\216" ));
        lblFl0->setText(QApplication::translate("AsiaWea", "2\347\272\247" ));
        lblFl3->setText(QApplication::translate("AsiaWea", "1\347\272\247" ));
    } // retranslateUi

};

namespace Ui {
    class AsiaWea: public Ui_AsiaWea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASIAWEA_H
