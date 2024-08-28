#include "Music.h"
#include "MusicListWidget.h"
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QProgressDialog>
#include <QDesktopServices>
#include <algorithm>

Music::Music(QUrl iurl)
{
    url=iurl;
    refreshInfo();
}

extern QString formatTime(qint64 timeMilliSeconds);
void Music::refreshInfo()
{
    QMediaPlayer tempPlayer;
    tempPlayer.setMedia(url);
    //元数据的解析需要时间，所以这里需要循环等待（但同时需要保持Qt事件处理机制在运行）
    while(!tempPlayer.isMetaDataAvailable()){
        QCoreApplication::processEvents();
    }
    QStringList list=tempPlayer.availableMetaData();//调试时查看有哪些元数据可用
    qDebug()<<list;
    if(tempPlayer.isMetaDataAvailable()){
        //歌曲信息
        author = tempPlayer.metaData(QStringLiteral("ContributingArtist")).toStringList().join(",");
        title = tempPlayer.metaData(QStringLiteral("Title")).toString();
        albumTitle = tempPlayer.metaData(QStringLiteral("AlbumTitle")).toString();
        audioBitRate = tempPlayer.metaData(QStringLiteral("AudioBitRate")).toInt();
        duration=tempPlayer.duration();
    }
}

QString Music::getLyricFile()
{
    QString mp3FilePath=url.toLocalFile();
    mp3FilePath.replace(".mp3",".lrc");
    mp3FilePath.replace(".flac",".lrc");
    mp3FilePath.replace(".mpga",".lrc");
    return mp3FilePath;
}

/*QString Music::getInfo() const
{
    return author+" - "+title+" ["+formatTime(duration)+"]";
}*/

QString Music::getInfo() const
{
    return author+" - "+title;
}

void Music::detail()
{
    QString info("歌曲名：%1\n艺术家：%2\n时长：%3\n唱片集：%4\n比特率：%5\n文件路径：%6");
    info=info.arg(title,author,formatTime(duration),albumTitle,QString::number(audioBitRate)+"bps",url.toString());
    QMessageBox::about(Q_NULLPTR,"歌曲信息",info);
}

void Music::insertSQL(const QString &name)
{
    QSqlQuery sql_query;
    QString insert_sql = "insert into MusicInfo values (?, ?, ?, ?, ?, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(name);
    sql_query.addBindValue(url.toString());
    sql_query.addBindValue(author);
    sql_query.addBindValue(title);
    sql_query.addBindValue(duration);
    sql_query.addBindValue(albumTitle);
    sql_query.addBindValue(audioBitRate);
    sql_query.exec();
}

MusicList::MusicList(const QList<QUrl> &urls, QString iname)
{
    addMusic(urls);
    setName(iname);
}

void MusicList::addMusic(const QList<QUrl> &urls)
{
    //实测这里耗时较长，所以添加一个进度显示对话框
    QProgressDialog proDialog(u8"添加进度",u8"取消",0,urls.size());
    proDialog.setMinimumSize(350,150);
    proDialog.setWindowModality(Qt::WindowModal);
    proDialog.setWindowTitle("添加中...请稍后");
    proDialog.show();
    int x=0;
    foreach (QUrl i, urls) {
        x++;
        proDialog.setValue(x);
        //过滤Url的类型
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(i.toLocalFile());
        if(mime.name()!="audio/mpeg"&&mime.name()!="audio/flac"){
            continue;
        }
        //剩下的符合类型
        music.push_back(Music(i));
        if(sql_flag){
            music[music.size()-1].insertSQL(name);
        }
        if(proDialog.wasCanceled()) break;
    }
}

void MusicList::addMusic(const Music &iMusic)
{
    music.push_back(iMusic);
    if(sql_flag){
        music[music.size()-1].insertSQL(name);
    }
}

Music MusicList::getMusic(int pos)
{
    return music[pos];
}

void MusicList::addToPlayList(QMediaPlaylist *playlist)
{
    for(auto i=music.begin();i!=music.end();i++){
        playlist->addMedia(i->getUrl());
    }
}

void MusicList::addToListWidget(MusicListWidget *listWidget)
{
    foreach(const Music &i,music){
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(listWidget->getIcon());
        item->setText(i.getInfo());
        QFont font = item->font();
        font.setPointSize(20);  // 设定为你想要的字体大小
        item->setFont(font);
        listWidget->addItem(item);
    }
}

void MusicList::removeMusic(int pos)
{
    if(sql_flag){
        remove_SQL_all();
        int i=0;
        for(auto it=music.begin();it!=music.end();){
            if(i==pos){
                it= music.erase(it);
                break;
            }
            else{
                it++;
            }
            i++;

        }
        insert_SQL_all();
    }else{
        int i=0;
        for(auto it=music.begin();it!=music.end();){
            if(i==pos){
                it= music.erase(it);
                break;
            }
            else{
                it++;
            }
            i++;

        }
    }

}

void MusicList::showInExplorer(int pos)
{
    QString str=music[pos].getUrl().toString();
    str.remove(str.split("/").last());//切割字符串获得所在的文件夹路径
    QDesktopServices::openUrl(str);
}

void MusicList::detail(int pos)
{
    music[pos].detail();
}

void MusicList::remove_SQL_all()
{
    QSqlQuery sql_query;
    QString delete_sql = "delete from MusicInfo where name = ?";
    sql_query.prepare(delete_sql);
    sql_query.addBindValue(name);
    sql_query.exec();
}

void MusicList::insert_SQL_all()
{
    for(auto i=music.begin();i!=music.end();i++){
        i->insertSQL(name);
    }
}

void MusicList::read_fromSQL()
{
    QSqlQuery sql_query;
    QString select_sql = "select url, author, title, duration, albumTitle, audioBitRate from MusicInfo where name = ?";
    sql_query.prepare(select_sql);
    sql_query.addBindValue(name);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            Music tempMusic;
            tempMusic.url=QUrl(sql_query.value(0).toString());
            tempMusic.author=sql_query.value(1).toString();
            tempMusic.title=sql_query.value(2).toString();
            tempMusic.duration=sql_query.value(3).toLongLong();
            tempMusic.albumTitle=sql_query.value(4).toString();
            tempMusic.audioBitRate=sql_query.value(5).toInt();
            music.push_back(tempMusic);
        }
    }
}

void MusicList::clear()
{
    music.clear();
    if(sql_flag){
        remove_SQL_all();
    }
}

MusicListWidget::MusicListWidget(QWidget *parent)
    :QListWidget (parent)
{

}

void MusicListWidget::refresh()
{
    clear();//先清空
    musicList.addToListWidget(this);
}

void MusicListWidget::setMusicList(const MusicList &music)
{
    musicList=music;
    refresh();
}

void MusicListWidget::setMusicList_playing(const MusicList &music)
{
    musicList=music;
    musicList.setSQL(false);
    refresh();
}

void MusicListWidget::removeMusic()
{
    int pos=currentRow();
    //移除
    musicList.removeMusic(pos);
    //实测这里如果直接使用removeMedia会导致当前播放的歌曲乱跳...（所以这里只能从更上层调用的地方来移除正在播放的歌曲）
    //playlist->removeMedia(pos);

    //从ListWidget中移除
    QListWidgetItem *tempItem=takeItem(pos);
    removeItemWidget(tempItem);
    delete tempItem;
}

void MusicListWidget::showInExplorer()
{
    int pos=currentRow();
    musicList.showInExplorer(pos);
}

void MusicListWidget::detail()
{
    int pos=currentRow();
    musicList.detail(pos);
}

int MusicList::indexOf(const Music &iMusic) const
{
    for (int i = 0; i < music.size(); ++i) {
        if (music[i].getUrl() == iMusic.getUrl()) {
            return i;  // 返回找到的索引
        }
    }
    return -1;  // 如果未找到，返回-1
}

bool MusicList::contains(const Music &iMusic) const
{
    for (const auto &musicItem : music) {
        if (musicItem.getUrl() == iMusic.getUrl()) {
            return true;
        }
    }
    return false;
}
