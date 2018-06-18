#include "updatebynetwork.h"

UpdateByNetwork::UpdateByNetwork(QObject *parent) : QObject(parent)
{
    bDownloaded = false;
    // pManager = new QNetworkAccessManager();
}

UpdateByNetwork::~UpdateByNetwork()
{
    if(pManager) delete pManager;
}

void UpdateByNetwork::setBaseAddress(QString s)         // 地址要有/结尾
{
    baseAddress = s;
}

void UpdateByNetwork::setlocalUpdateDir(QString d)      // 设置升级文件的下载目录
{
    localUpdateDir = d;
}

void UpdateByNetwork::setDownloadFileName(QString f)    //纯文件名
{
    downloadFileName = f;
}

void UpdateByNetwork::startDownload()
{
    // 开始下载
    bDownloaded = false;
    QUrl url = QUrl(baseAddress + downloadFileName);
    // qDebug() << url.toString();

    QDir mydir = QDir::current();
//    qDebug() << mydir.absolutePath();
    if(!mydir.exists(localUpdateDir))
    {
        mydir.mkdir(localUpdateDir);    // 将文件下载到exe目录/update/下
//        qDebug() << localUpdateDir << "not exists. 需要创建";
    }

//    QString strAppDir = QCoreApplication::applicationFilePath();
//    strAppDir = strAppDir.left(strAppDir.lastIndexOf("/"));
//    QDir mydir(strAppDir);    // = QDir::current();
//    // qDebug() << mydir.absolutePath();
//    mydir.mkdir(LOCALUPDATEDIR);        // 将文件下载到指定目录

    pFile = new QFile(localUpdateDir + "/" + downloadFileName);
    if(!pFile->open(QIODevice::WriteOnly | QIODevice::Truncate)){
         qDebug() << "本地文件无法创建，无法下载文件。";
         return;
    }
    pManager = new QNetworkAccessManager();     //必须和pReply生成放在一起，否则多线程时出错。
    pReply = pManager->get(QNetworkRequest(url));
    // 创建信号机制
    //connect(pReply, QNetworkReply::downloadProgress, this, myDownloadProgress);
    connect(pReply, QNetworkReply::finished, this, UpdateByNetwork::myFinished);
    return;
}

void UpdateByNetwork::myFinished()
{
    // qDebug() << "myFinished开始。";
    if(pFile){
         pFile->write(pReply->readAll());
         pFile->flush();
         pFile->close();
         delete pFile;
         pFile = nullptr;
    }
    pReply->deleteLater();
    pReply = nullptr;
    bDownloaded = true;
    qDebug() << "myFinished结束。";
    // QMessageBox::information(this, "完成", "本地下载完成")；
}

bool UpdateByNetwork::isDownloaded()
{
    return bDownloaded;
}
