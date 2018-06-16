#ifndef UPDATEBYNETWORK_H
#define UPDATEBYNETWORK_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QUrl>
#include <QString>
#include <QDir>
#include <QtNetwork>

class UpdateByNetwork : public QObject
{
    Q_OBJECT
public:
    explicit UpdateByNetwork(QObject *parent = 0);
    ~UpdateByNetwork();

    // setter
    void setBaseAddress(QString);
    void setDownloadFileName(QString);

    void startDownload();    // "开始下载"配置参数函数
    bool isDownloaded();


signals:

private slots:
    void myFinished();      // "完成下载"，进行清理工作

private:
    QFile *pFile;       // 处理文件信息的handler

    const QString LOCALUPDATEDIR = "update";    // "update"目录
    QString baseAddress;        // 指定基地址，如：http://abc.com/(需要加文件名)
    QString downloadFileName;   // 指定文件名

    // 进行网络通信的handler
    QNetworkAccessManager *pManager;
    QNetworkReply *pReply;

public:
    volatile bool bDownloaded;   // "是否下载"标志
};

#endif // UPDATEBYNETWORK_H
