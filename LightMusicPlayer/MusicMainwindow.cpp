#include "MusicMainwindow.h"
#include "ui_MusicMainwindow.h"
#include <QPainter>
#include "myQSS.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QScrollBar>
#include "Music.h"
#include <QMouseEvent>
#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>
#include "MusicListDialog.h"
#include "rotatablelabel.h"
#include <QGraphicsBlurEffect>
#include "CustomWidget.h"

MusicMainwindow::MusicMainwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    //UI初始化（.ui文件中无法完成的设置，这里补上）
    init_UI();

    //初始化生成播放器模块及相关组件
    init_play();
    
    //右键菜单初始化
    init_actions();
    
    //数据库初始化
    init_sqlite();
    
    //歌单初始化
    init_musicList();
    
    //配置初始化
    init_settings();
    
    //系统托盘初始化
    init_systemTrayIcon();


    // 初始化旋转动画
    rotationAnimation = new QPropertyAnimation(ui->coverLabel, "rotation");
    rotationAnimation->setDuration(20000); // 一圈20秒
    rotationAnimation->setStartValue(0);
    rotationAnimation->setEndValue(360);
    rotationAnimation->setLoopCount(-1); // 无限循环

    ui->label_background->setStyleSheet("background-color: rgb(18,47,86);");
    ui->positionSlider->setVisible(false);
    ui->positionLabel->setVisible(false);
    ui->infoLabel->setText("");

}

MusicMainwindow::~MusicMainwindow()
{
    delete ui;
}
void MusicMainwindow::paintEvent(QPaintEvent *event)
{
    //需要添加以下代码，才能正常在主窗口Widget中显示背景图片（https://blog.csdn.net/xiejie0226/article/details/81165379）
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void MusicMainwindow::init_UI()
{
    //窗口设置圆角后，会出现留白，需要添加以下代码
    setAttribute(Qt::WA_TranslucentBackground, true);
    //去除标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    
    //UI初始化（.ui文件中无法完成的设置，这里补上）

    ui->MusicDetail_Title->setText("");
    ui->MUsicDetail_Singer->setText("");
    //ui->MusicDetail_AlbumTitle->setText("");

    ui->volumeSlider->hide();
    ui->playListWidget->verticalScrollBar()->setStyleSheet(ListWidgetScrollBarStyle());
    ui->favorMusicWidget->verticalScrollBar()->setStyleSheet(ListWidgetScrollBarStyle());
    ui->localMusicWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->playListWidget->setStyleSheet(ListWidgetStyle());
    ui->favorMusicWidget->setStyleSheet(ListWidgetStyle());
    ui->localMusicWidget->setStyleSheet(ListWidgetStyle());
    ui->playListWidget->setIcon(QIcon(":/image/image/image/music.png"));
    ui->favorMusicWidget->setIcon(QIcon(":/image/image/image/like.png"));
}

void MusicMainwindow::init_play()
{
    //播放器初始化
    player= new QMediaPlayer(this);
    playlist=new QMediaPlaylist;    
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    //连接槽与信号
    connect(ui->positionSlider, &QAbstractSlider::valueChanged, this, &MusicMainwindow::setPosition);
    connect(player, &QMediaPlayer::positionChanged, this, &MusicMainwindow::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicMainwindow::updateDuration);
    connect(player, &QMediaPlayer::metaDataAvailableChanged, this, &MusicMainwindow::updateInfo);
    connect(player, &QMediaPlayer::stateChanged, this, &MusicMainwindow::updatePlayBtn);
}

void MusicMainwindow::init_actions()
{
    //“当前播放”列表右键菜单初始化
    ui->playListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_playlist_delete=new QAction(QIcon(":/image/image/image/remove.png"),u8"移除");
    connect(action_playlist_delete,&QAction::triggered,this,&MusicMainwindow::playlist_removeMusic);
    QAction *action_playlist_showfile=new QAction(QIcon(":/image/image/image/music-dir.png"),u8"打开所在文件夹");
    connect(action_playlist_showfile,&QAction::triggered,ui->playListWidget,&MusicListWidget::showInExplorer);
    QAction *action_playlist_detail=new QAction(QIcon(":/image/image/image/detail.png"),u8"歌曲详情");
    connect(action_playlist_detail,&QAction::triggered,ui->playListWidget,&MusicListWidget::detail);
    QAction *action_play_to_favor=new QAction(QIcon(":/image/image/image/To-like.png"),u8"添加到我喜欢");
    connect(action_play_to_favor,&QAction::triggered,this,&MusicMainwindow::play_to_favor);
    menu_playlist=new QMenu(this);
    menu_playlist->addAction(action_playlist_delete);
    menu_playlist->addAction(action_playlist_showfile);
    menu_playlist->addAction(action_playlist_detail);
    menu_playlist->addAction(action_play_to_favor);
    
    //“本地音乐”列表右键菜单初始化
    ui->localMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_locallist_delete=new QAction(QIcon(":/image/image/image/remove.png"),u8"移除");
    connect(action_locallist_delete,&QAction::triggered,ui->localMusicWidget,&MusicListWidget::removeMusic);
    QAction *action_locallist_showfile=new QAction(QIcon(":/image/image/image/music-dir.png"),u8"打开所在文件夹");
    connect(action_locallist_showfile,&QAction::triggered,ui->localMusicWidget,&MusicListWidget::showInExplorer);
    QAction *action_locallist_detail=new QAction(QIcon(":/image/image/image/detail.png"),u8"歌曲详情");
    connect(action_locallist_detail,&QAction::triggered,ui->localMusicWidget,&MusicListWidget::detail);
    QAction *action_local_to_favor=new QAction(QIcon(":/image/image/image/To-like.png"),u8"添加到我喜欢");
    connect(action_local_to_favor,&QAction::triggered,this,&MusicMainwindow::local_to_favor);
    QAction *action_local_to_playlist=new QAction(QIcon(":/image/image/image/To-playlist.png"),u8"添加到当前播放列表");
    connect(action_local_to_playlist,&QAction::triggered,this,&MusicMainwindow::local_to_playlist);
    menu_locallist=new QMenu(this);
    menu_locallist->addAction(action_locallist_delete);
    menu_locallist->addAction(action_locallist_showfile);
    menu_locallist->addAction(action_locallist_detail);
    menu_locallist->addAction(action_local_to_favor);
    menu_locallist->addAction(action_local_to_playlist);

    
    //“我喜欢”列表右键菜单初始化
    ui->favorMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_favorlist_delete = new QAction(QIcon(":/image/image/image/remove.png"),u8"移除");
	connect(action_favorlist_delete, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::removeMusic);
	QAction *action_favorlist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"),u8"打开所在文件夹");
	connect(action_favorlist_showfile, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::showInExplorer);
	QAction *action_favorlist_detail = new QAction(QIcon(":/image/image/image/detail.png"),u8"歌曲详情");
	connect(action_favorlist_detail, &QAction::triggered, ui->favorMusicWidget, &MusicListWidget::detail);
    QAction *action_favor_to_playlist=new QAction(QIcon(":/image/image/image/To-playlist.png"),u8"添加到当前播放列表");
    connect(action_favor_to_playlist,&QAction::triggered,this,&MusicMainwindow::favor_to_playlist);
	menu_favorlist = new QMenu(this);
	menu_favorlist->addAction(action_favorlist_delete);
	menu_favorlist->addAction(action_favorlist_showfile);
	menu_favorlist->addAction(action_favorlist_detail);
    menu_favorlist->addAction(action_favor_to_playlist);
    
 /*   //“我的歌单”列表右键菜单初始化
    ui->nameListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_namelist_delete=new QAction(QIcon(":/image/image/image/remove.png"),u8"删除");
    connect(action_namelist_delete,&QAction::triggered,this,&MainWidget::namelist_delete);
    menu_namelist=new QMenu(this);
    menu_namelist->addAction(action_namelist_delete);
    
    //歌单展示列表右键菜单初始化
    ui->musicListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_musiclist_delete = new QAction(QIcon(":/image/image/image/remove.png"), u8"移除");
    connect(action_musiclist_delete, &QAction::triggered, this, &MainWidget::musiclist_removeMusic);
    QAction *action_musiclist_showfile = new QAction(QIcon(":/image/image/image/music-dir.png"), u8"打开所在文件夹");
    connect(action_musiclist_showfile, &QAction::triggered, ui->musicListWidget, &MusicListWidget::showInExplorer);
    QAction *action_musiclist_detail = new QAction(QIcon(":/image/image/image/detail.png"), u8"歌曲详情");
    connect(action_musiclist_detail, &QAction::triggered, ui->musicListWidget, &MusicListWidget::detail);
    QAction *action_musiclist_to_favor = new QAction(QIcon(":/image/image/image/To-like.png"), u8"添加到我喜欢");
    connect(action_musiclist_to_favor, &QAction::triggered, this, &MainWidget::musiclist_to_favor);
    QAction *action_musiclist_to_playlist = new QAction(QIcon(":/image/image/image/To-playlist.png"), u8"添加到当前播放列表");
    connect(action_musiclist_to_playlist, &QAction::triggered, this, &MainWidget::musiclist_to_playlist);
    menu_musiclist = new QMenu(this);
    menu_musiclist->addAction(action_musiclist_delete);
    menu_musiclist->addAction(action_musiclist_showfile);
    menu_musiclist->addAction(action_musiclist_detail);
    menu_musiclist->addAction(action_musiclist_to_favor);
    menu_musiclist->addAction(action_musiclist_to_playlist);

    //“换肤”的菜单项
    QAction *action_backgroud_to_default = new QAction(QIcon(":/image/image/image/default.png"),u8"更换到默认背景");
	connect(action_backgroud_to_default,&QAction::triggered,this,&MainWidget::background_to_default);
    QAction *action_backgroud_setting=new QAction(QIcon(":/image/image/image/setting.png"),u8"自定义背景");
    connect(action_backgroud_setting,&QAction::triggered,this,&MainWidget::background_setting);
    menu_changeSkin=new QMenu(this);
    menu_changeSkin->addAction(action_backgroud_to_default);
    menu_changeSkin->addAction(action_backgroud_setting);*/
}

void MusicMainwindow::init_sqlite()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Music.db");
        database.setUserName("NJUTang");
        database.setPassword("123456");
        if (!database.open())
        {
            QMessageBox::critical(this,"无法打开数据库文件：Music.db",database.lastError().databaseText());
            exit(-1);
        }
    }
    //检查两个表是否存在，不存在则创建不存在的表
    QSqlQuery query;
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("MusicInfo"));
    if(query.next()){
        if(query.value(0).toInt()==0){
            QSqlQuery sql_query;
            //QString create_sql = "create table MusicInfo (id int primary key, name varchar(30), url varchar(200), author varchar(50), title varchar(50), duration bigint, albumTitle varchar(50), audioBitRate int)";
            QString create_sql = "create table MusicInfo (name varchar(30), url varchar(200), author varchar(50), title varchar(50), duration bigint, albumTitle varchar(50), audioBitRate int)";
            sql_query.prepare(create_sql);
            sql_query.exec();
        }        
    }
    QSqlQuery query2;
    query2.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("MusicLists"));
    if(query2.next()){
        if(query2.value(0).toInt()==0){
            QSqlQuery sql_query;
            QString create_sql = "create table MusicLists (name varchar(30))";
            sql_query.prepare(create_sql);
            sql_query.exec();
        }        
    }
}

void MusicMainwindow::init_musicList()
{
    //本地音乐 初始化
    ui->localMusicWidget->musicList.setName("LocalMusic");
    ui->localMusicWidget->musicList.read_fromSQL();
    ui->localMusicWidget->refresh();

    //我喜欢 初始化
    ui->favorMusicWidget->musicList.setName("FavorMusic");
    ui->favorMusicWidget->musicList.read_fromSQL();
    ui->favorMusicWidget->refresh();
    
}

void MusicMainwindow::updateBackground(const QPixmap &coverPixmap)
{

    /*if (coverPixmap.isNull()) {
            qDebug() << "updateBackground: Cover pixmap is null, setting background color.";
            ui->label_background->setStyleSheet("background-color: rgb(18,47,86);");
            ui->label_background->clear();  // 清除之前的图片，确保只显示颜色
            return;
        }
    // 将封面图片设置为背景图片
    QPixmap blurredPixmap = coverPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 创建一个新的 QPixmap 用于模糊效果
    QPixmap backgroundPixmap = blurredPixmap;
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(10); // 设置模糊半径

    ui->label_background->setPixmap(backgroundPixmap);
    ui->label_background->setScaledContents(true);
    ui->label_background->setGraphicsEffect(blurEffect);
    //ui->label_background->setGeometry(0, 0, width(), height());

    ui->label_background->setStyleSheet("");  // 清除背景颜色样式

    // 重新调整布局
    //ui->label_background->show();*/
}

void MusicMainwindow::init_settings()
{
    /*QSettings mysettings("./LightMusicPlayer.ini",QSettings::IniFormat);
    mysettings.setIniCodec("UTF8");
    QString fileName = mysettings.value("background/image-url").toString();
    QImage testImage(fileName);
    if(!fileName.isEmpty()&&!testImage.isNull())
    {
        setStyleSheet(QString("QWidget#Widget{"
                              "border-radius:10px;"
                              "border-image: url(%1);}").arg(fileName));
    }else{
        fileName=":/image/image/background/bg3.jpg";
        QSettings mysettings("./LightMusicPlayer.ini",QSettings::IniFormat);
        mysettings.setIniCodec("UTF8");
        mysettings.setValue("background/image-url",fileName);
        setStyleSheet(QString("QWidget#Widget{"
                              "border-radius:10px;"
                              "border-image: url(%1);}").arg(fileName));
    }*/
}


void MusicMainwindow::on_playListWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->playListWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_playlist->exec(QCursor::pos());
}

void MusicMainwindow::playlist_removeMusic()
{
    int pos=ui->playListWidget->currentRow();
    int playing_pos=playlist->currentIndex();
    ui->playListWidget->removeMusic();
    if(pos<playing_pos){
        //移除前备份
        QMediaPlayer::State state=player->state();//记录下删除前的播放状态
        qint64 position_backup=player->position();
        playing_pos=playing_pos-1;
        playlist->removeMedia(pos);
        //移除后恢复
        playlist->setCurrentIndex(playing_pos);
        player->setPosition(position_backup);
        ui->positionSlider->setValue(position_backup);
        if(state==QMediaPlayer::PlayingState){
            player->play();
        }else if(state==QMediaPlayer::PlayingState){
            player->pause();
        }
    }else if(pos<playing_pos){
        playlist->removeMedia(pos);
        playlist->setCurrentIndex(pos);
    }else{
        playlist->removeMedia(pos);
    }
}

void MusicMainwindow::play_to_favor()
{
    int pos=ui->playListWidget->currentRow();
    ui->favorMusicWidget->musicList.addMusic(ui->playListWidget->musicList.getMusic(pos));
    ui->favorMusicWidget->refresh();
}

void MusicMainwindow::favor_to_playlist()
{
    int pos=ui->favorMusicWidget->currentRow();
    Music tempMusic=ui->favorMusicWidget->musicList.getMusic(pos);
    ui->playListWidget->musicList.addMusic(tempMusic);
    ui->playListWidget->refresh();
    //添加到播放器
    playlist->addMedia(tempMusic.getUrl());
}


void MusicMainwindow::local_to_favor()
{
    int pos=ui->localMusicWidget->currentRow();
    ui->favorMusicWidget->musicList.addMusic(ui->localMusicWidget->musicList.getMusic(pos));
    ui->favorMusicWidget->refresh();
}

void MusicMainwindow::local_to_playlist()
{
    int pos=ui->localMusicWidget->currentRow();
    Music tempMusic=ui->localMusicWidget->musicList.getMusic(pos);
    ui->playListWidget->musicList.addMusic(tempMusic);
    ui->playListWidget->refresh();
    //添加到播放器
    playlist->addMedia(tempMusic.getUrl());
}





void MusicMainwindow::on_localMusicWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->localMusicWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_locallist->exec(QCursor::pos());
}

void MusicMainwindow::on_favorMusicWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->favorMusicWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_favorlist->exec(QCursor::pos());
}


QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MusicMainwindow::updatePosition(qint64 position)
{
    ui->positionSlider->setValue(static_cast<int>(position));
    ui->positionLabel->setText(formatTime(position)+"/"+formatTime(player->duration()));
    if(playlist->currentIndex()>=0)ui->lyricWidget->show(position);
}

void MusicMainwindow::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0, static_cast<int>(duration));
    ui->positionSlider->setEnabled(static_cast<int>(duration) > 0);
    if(!(static_cast<int>(duration) > 0)) {
        //无音乐播放时，界面元素
        //ui->infoLabel->setText("LET'S START A BEAUTIFUL DAY");
        mySystemTray->setToolTip(u8"LightMusicPlayer");

        QPixmap pixmap(":/image/image/image/non-music.png");

        ui->coverLabel->setPixmap(pixmap);

        ui->MusicDetail_Title->setText("");
        ui->MUsicDetail_Singer->setText("");
        //ui->MusicDetail_AlbumTitle->setText("");
        ui->lyricWidget->clear();
    }
    ui->positionSlider->setPageStep(static_cast<int>(duration) / 10);
}

void MusicMainwindow::setPosition(int position)
{
    // avoid seeking when the slider value change is triggered from updatePosition()
    if (qAbs(player->position() - position) > 99)
        player->setPosition(position);
}

void MusicMainwindow::updateInfo()
{
    if (player->isMetaDataAvailable()) {
        //返回可用MP3元数据列表（调试时可以查看）
        QStringList availableMetaData = player->availableMetaData();
        qDebug() << "Available metadata keys:" << availableMetaData;
        QStringList listInfo_debug=player->availableMetaData();
        //歌曲信息
        QString info="";
        QString author = player->metaData(QStringLiteral("ContributingArtist")).toStringList().join(",");
        info.append(author);
        QString title = player->metaData(QStringLiteral("Title")).toString();
        QString albumTitle = player->metaData(QStringLiteral("AlbumTitle")).toString();
        info.append(" - "+title);
        author.append(" - "+albumTitle);
        //info.append(" ["+formatTime(player->duration())+"]");
        ui->infoLabel->setText(info);
        //ui->MusicDetail_Title->setPixmap(QPixmap(":/image/image/image/music1.png"));
        ui->MusicDetail_Title->setText(title);
        //ui->MUsicDetail_Singer->setPixmap(QPixmap(":/image/image/image/singer.png"));
        ui->MUsicDetail_Singer->setText(author);
        //ui->MusicDetail_AlbumTitle->setPixmap(QPixmap(":/image/image/image/album.png"));
        //ui->MusicDetail_AlbumTitle->setText(albumTitle);

        mySystemTray->setToolTip("正在播放："+info);
        //封面图片（应获取"ThumbnailImage" From: https://www.zhihu.com/question/36859497）
        QImage picImage= player->metaData(QStringLiteral("CoverArtImage")).value<QImage>();
        if(picImage.isNull())
        {picImage=QImage(":/image/image/image/non-music.png");
         qDebug() << "ThumbnailImage not available, using default image.";}

        QPixmap pixmap(QPixmap::fromImage(picImage));


        ui->coverLabel->setPixmap(pixmap);
        ui->coverLabel->setScaledContents(true);
        //圆形显示
        QRegion maskRegion = QRegion(0, 0, 350, 350, QRegion::Ellipse);
        ui->coverLabel->setMask(maskRegion);

        QPixmap blurredPixmap = pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // 创建一个新的 QPixmap 用于模糊效果
        QPixmap backgroundPixmap = blurredPixmap;
        QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurRadius(20); // 设置模糊半径

        ui->label_background->setPixmap(backgroundPixmap);
        ui->label_background->setScaledContents(true);
        ui->label_background->setGraphicsEffect(blurEffect);
        //ui->label_background->setGeometry(0, 0, width(), height());

        ui->label_background->setStyleSheet("");  // 清除背景颜色样式


        //改变正在播放歌曲的图标
        for(int i=0;i<playlist->mediaCount();i++){
            QListWidgetItem *p=ui->playListWidget->item(i);
            p->setIcon(ui->playListWidget->getIcon());
        }
        int index=playlist->currentIndex();
        QListWidgetItem *p=ui->playListWidget->item(index);
        p->setIcon(QIcon(":/image/image/image/music-playing.png"));
        
        //歌词界面显示的信息
        /*ui->musicTitleLabel->setText(title);
        ui->musicAlbumLabel->setText(u8"专辑："+albumTitle);
        ui->musicAuthorLabel->setText(u8"歌手："+author);*/
        //解析歌词
        ui->lyricWidget->process(ui->playListWidget->musicList.music[index].getLyricFile());

        updateFavoriteButtonState();
        updateUIForPlayback();
    }
}

void MusicMainwindow::updatePlayBtn()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {
        ui->btnPlay->setStyleSheet(PlayStyle());
        action_systemTray_play->setIcon(QIcon(":/image/image/image/pause2.png"));
        action_systemTray_play->setText(u8"暂停");
    }else{
        ui->btnPlay->setStyleSheet(PaseStyle());
        action_systemTray_play->setIcon(QIcon(":/image/image/image/play2.png"));
        action_systemTray_play->setText(u8"播放");
    }
}

void MusicMainwindow::systemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        //显/隐主界面
        if(isHidden()){
            show();
        }else{
            hide();
        }
        break;
    default:
        break;
    }
}

void MusicMainwindow::quitMusicPlayer()
{
    //退出应用
    QCoreApplication::quit();
}

void MusicMainwindow::init_systemTrayIcon()
{
    mySystemTray=new QSystemTrayIcon(this);
    mySystemTray->setIcon(QIcon(":/image/image/image/systemTrayIcon.png"));
    mySystemTray->setToolTip(u8"LightMusicPlayer");
    connect(mySystemTray,&QSystemTrayIcon::activated,this,&MusicMainwindow::systemTrayIcon_activated);
    //添加菜单项
    QAction *action_systemTray_pre = new QAction(QIcon(":/image/image/image/pre2.png"), u8"上一首");
    connect(action_systemTray_pre, &QAction::triggered, this, &MusicMainwindow::on_btnPre_clicked);
    action_systemTray_play = new QAction(QIcon(":/image/image/image/play2.png"), u8"播放");
    connect(action_systemTray_play, &QAction::triggered, this, &MusicMainwindow::on_btnPlay_clicked);
    QAction *action_systemTray_next = new QAction(QIcon(":/image/image/image/next2.png"), u8"下一首");
    connect(action_systemTray_next, &QAction::triggered, this, &MusicMainwindow::on_btnNext_clicked);
    action_systemTray_playmode = new QAction(QIcon(":/image/image/image/loop2.png"), u8"循环播放");
    connect(action_systemTray_playmode, &QAction::triggered, this, &MusicMainwindow::on_btnPlayMode_clicked);
    QAction *action_systemTray_quit = new QAction(QIcon(":/image/image/image/exit.png"), u8"退出应用");
    connect(action_systemTray_quit, &QAction::triggered, this, &MusicMainwindow::quitMusicPlayer);
    
    QMenu *pContextMenu = new QMenu(this);
    pContextMenu->addAction(action_systemTray_pre);
    pContextMenu->addAction(action_systemTray_play);
    pContextMenu->addAction(action_systemTray_next);
    pContextMenu->addAction(action_systemTray_playmode);
    pContextMenu->addAction(action_systemTray_quit);
    mySystemTray->setContextMenu(pContextMenu);
    mySystemTray->show();
}

void MusicMainwindow::mousePressEvent(QMouseEvent *event)
{
    //实现点击界面中某点，音量条隐藏
    int x=event->pos().x();
    int y=event->pos().y();
    int x_widget=ui->volumeSlider->geometry().x(); //注：这里“wiget”的名字是要从UI文件编译后生成的ui_xxx.h文件中得知（在UI布局中看不到）
    int y_widget=ui->volumeSlider->geometry().y();
    int w=ui->volumeSlider->geometry().width();
    int h=ui->volumeSlider->geometry().height();
    if(!(x>=x_widget&&x<=x_widget+w && y>=y_widget&&y<=y_widget+h)){
        ui->volumeSlider->hide();
    }
    
    //记录窗口移动的初始位置
    offset = event->globalPos() - pos();
    event->accept();
}

void MusicMainwindow::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->pos().x();
    int y=event->pos().y();
    //注：这里“layoutWidget1”的名字是要从UI文件编译后生成的ui_xxx.h文件中得知（在UI布局中看不到）
    if((y<ui->titleLabel->geometry().height())&&(x<1100)){
        move(event->globalPos() - offset);
        event->accept();    
        setCursor(Qt::ClosedHandCursor);
    }
}

void MusicMainwindow::mouseReleaseEvent(QMouseEvent *event)
{
    offset = QPoint();
    event->accept();
    setCursor(Qt::ArrowCursor);
}

void MusicMainwindow::closeEvent(QCloseEvent *event)
{
    //最小化到托盘
    if(!mySystemTray->isVisible()){
        mySystemTray->show();
    }
    hide();
    event->ignore();
}

void MusicMainwindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MusicMainwindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    ui->localMusicWidget->musicList.addMusic(urls);
    ui->localMusicWidget->refresh();
    ui->stackedWidget->setCurrentIndex(0);//切换到本地音乐
    
}
void MusicMainwindow::on_btnQuit_clicked()
{
    close();
}

void MusicMainwindow::on_btnPlay_clicked()
{
    if(player->state()==QMediaPlayer::PlayingState)
     {
        player->pause();
        rotationAnimation->pause();
     }
    else if(player->state()==QMediaPlayer::PausedState){

        player->play();
        rotationAnimation->start();
    }
    else if(!playlist->isEmpty() && (player->state()==QMediaPlayer::StoppedState))
    {
        playlist->setCurrentIndex(0);
        player->play();
        rotationAnimation->start();
    }
}

void MusicMainwindow::on_btnNext_clicked()
{
    // 停止旋转动画
    rotationAnimation->stop();
    // 将封面图片旋转角度重置为 0°
    ui->coverLabel->setRotation(0);

    playlist->next();

    rotationAnimation->start();
}

void MusicMainwindow::on_btnPre_clicked()
{
    // 停止旋转动画
    rotationAnimation->stop();
    // 将封面图片旋转角度重置为 0°
    ui->coverLabel->setRotation(0);

    playlist->previous();

    rotationAnimation->start();
}

void MusicMainwindow::on_btnPlayMode_clicked()
{
    if(playlist->playbackMode()==QMediaPlaylist::Loop){
        ui->btnPlayMode->setStyleSheet(RandomStyle());
        ui->btnPlayMode->setToolTip(u8"随机播放");
        action_systemTray_playmode->setIcon(QIcon(":/image/image/image/random2.png"));
        action_systemTray_playmode->setText(u8"随机播放");
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else if(playlist->playbackMode()==QMediaPlaylist::Random){
        ui->btnPlayMode->setStyleSheet(LoopOneStyle());
        ui->btnPlayMode->setToolTip(u8"单曲循环");
        action_systemTray_playmode->setIcon(QIcon(":/image/image/image/loop-one2.png"));
        action_systemTray_playmode->setText(u8"单曲循环");
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if(playlist->playbackMode()==QMediaPlaylist::CurrentItemInLoop){
        ui->btnPlayMode->setStyleSheet(LoopStyle());
        ui->btnPlayMode->setToolTip(u8"循环播放");
        action_systemTray_playmode->setIcon(QIcon(":/image/image/image/loop2.png"));
        action_systemTray_playmode->setText(u8"循环播放");
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void MusicMainwindow::on_btnMin_clicked()
{
    showMinimized();//窗口最小化
}

void MusicMainwindow::on_btnAdd_clicked()
{    
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setWindowTitle(tr("添加本地音乐（注：自动过滤，按下\"Ctrl+A\"全选添加即可；不支持添加文件夹）"));
    QStringList list;list<<"application/octet-stream";
    fileDialog.setMimeTypeFilters(list);
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted){
       QList<QUrl> urls=fileDialog.selectedUrls();
       ui->localMusicWidget->musicList.addMusic(urls);
       ui->localMusicWidget->refresh();
       ui->stackedWidget->setCurrentIndex(1);//切换到本地音乐
    }
}

void MusicMainwindow::on_btnVolume_clicked()
{
    if(ui->volumeSlider->isHidden()){
        ui->volumeSlider->show();
    }else{
        ui->volumeSlider->hide();
    }
}

void MusicMainwindow::on_btnCurMusic_clicked()
{
    //切换到“当前播放”界面
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicMainwindow::on_btnLocalMusic_clicked()
{
    //切换到“本地音乐”界面
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicMainwindow::on_btnFavorMusic_clicked()
{
    //切换到“我喜欢”界面
    ui->stackedWidget->setCurrentIndex(2);
}

void MusicMainwindow::on_playListWidget_doubleClicked(const QModelIndex &index)
{
    int i=index.row();
    playlist->setCurrentIndex(i);
    player->play();
    updateFavoriteButtonState();
}

void MusicMainwindow::on_localMusicWidget_doubleClicked(const QModelIndex &index)
{
    playlist->clear();
    ui->localMusicWidget->musicList.addToPlayList(playlist);
    ui->playListWidget->setMusicList_playing(ui->localMusicWidget->musicList);
    int i=index.row();
    playlist->setCurrentIndex(i);

    rotationAnimation->stop();
    //复原旋转状态
    ui->coverLabel->setRotation(0);
    player->play();

    rotationAnimation->start();
    ui->stackedWidget->setCurrentIndex(0);//跳转到当前播放列表
    updateFavoriteButtonState();
}

void MusicMainwindow::on_favorMusicWidget_doubleClicked(const QModelIndex &index)
{
    playlist->clear();
    ui->favorMusicWidget->musicList.addToPlayList(playlist);
    ui->playListWidget->setMusicList_playing(ui->favorMusicWidget->musicList);
    int i=index.row();
    playlist->setCurrentIndex(i);

    rotationAnimation->stop();
    //复原旋转状态
    ui->coverLabel->setRotation(0);
    player->play();

    rotationAnimation->start();
    ui->stackedWidget->setCurrentIndex(0);//跳转到当前播放列表
    updateFavoriteButtonState();
}

void MusicMainwindow::on_btnAddtoFavor_clicked()
{
    MusicListDialog *dialog=new MusicListDialog(this);
    int num=ui->localMusicWidget->count();
    bool *results=new bool[num];
    dialog->setMusicList(ui->localMusicWidget->musicList,results);
    if(dialog->exec()==QDialog::Accepted){
        for(int i=0;i<num;i++){
            if(results[i]){
                ui->favorMusicWidget->musicList.addMusic(ui->localMusicWidget->musicList.getMusic(i));
            }
        }
        ui->favorMusicWidget->refresh();
    }
    delete []results;
}


void MusicMainwindow::on_btnClear_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->playListWidget->musicList.clear();
        ui->playListWidget->refresh();
        playlist->clear();
    }
}

void MusicMainwindow::on_btnClear_2_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->localMusicWidget->musicList.clear();
        ui->localMusicWidget->refresh();
    }
}

void MusicMainwindow::on_btnClear_3_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "此操作不可逆！\n确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        ui->favorMusicWidget->musicList.clear();
        ui->favorMusicWidget->refresh();
    }
}



void MusicMainwindow::on_btnMusicDetails_clicked()
{
    //切换到“本地音乐”界面
    ui->stackedWidget->setCurrentIndex(3);
}

void MusicMainwindow::on_volumeSlider_valueChanged(int value)
{
    player->setVolume(value);
}

void MusicMainwindow::on_music_like_or_not_clicked()
{
    QUrl currentUrl = player->currentMedia().canonicalUrl();
    Music currentMusic(currentUrl);

    int pos = ui->favorMusicWidget->musicList.indexOf(currentMusic);


    if (pos != -1) {
        // 如果歌曲在“我喜欢的”列表中，删除它
        ui->favorMusicWidget->musicList.removeMusic(pos);
        ui->music_like_or_not->setIcon(QIcon(":/image/image/image/not_liked.png"));  // 更改按钮图标为未收藏状态
    } else {
        // 如果歌曲不在“我喜欢的”列表中，添加它
        ui->favorMusicWidget->musicList.addMusic(currentMusic);
        ui->music_like_or_not->setIcon(QIcon(":/image/image/image/liked.png"));  // 更改按钮图标为已收藏状态
    }

    // 刷新“我喜欢的”列表
    ui->favorMusicWidget->refresh();
}

void MusicMainwindow::updateFavoriteButtonState()
{
    // 获取当前播放的歌曲
    int currentIndex = playlist->currentIndex();
    if (currentIndex < 0 || currentIndex >= playlist->mediaCount()) {
        return; // 防止越界
    }

    QMediaContent currentMedia = playlist->media(currentIndex);
    Music currentMusic(currentMedia.canonicalUrl());

    int pos = ui->favorMusicWidget->musicList.indexOf(currentMusic);

    if (pos != -1) {
        // 如果歌曲在“我喜欢的”列表中
        ui->music_like_or_not->setIcon(QIcon(":/image/image/image/liked.png"));  // 更改按钮图标为未收藏状态
    } else {
        // 如果歌曲不在“我喜欢的”列表中
        ui->music_like_or_not->setIcon(QIcon(":/image/image/image/not_liked.png"));  // 更改按钮图标为已收藏状态
    }
}

void MusicMainwindow::updateUIForPlayback()
{
    // 如果播放列表为空
    if (playlist->mediaCount()<=0) {
        ui->positionSlider->setVisible(false);
        ui->positionLabel->setVisible(false);
        ui->infoLabel->setVisible(false);
    } else {
        // 如果播放列表有歌曲
        ui->positionSlider->setVisible(true);
        ui->positionLabel->setVisible(true);
        ui->infoLabel->setVisible(true);
    }
}
