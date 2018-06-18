#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    //ui->lineEdit->setText("abc");
//    ui->progressBar->setValue(0);
//    isLogin = true;

//    //2. 读入升级程序
//    /////////////////////
//    pUpdate = new UpdateByNetwork();
//    pUpdate->setBaseAddress("http://localhost:8080/download/");
//    pUpdate->setDownloadFileName("setup.exe");


//    pUpdate->startDownload();
//    // QMessageBox::information(this, "下载", "正在下载。");  //法1：可以挂起当前 dialog 线程，可以实现下载线程完成下载。
//    // qDebug() <<"dialog中";
//    while (!pUpdate->bDownloaded) {      //法2：可以等待下载线程完成下载。
//        QCoreApplication::processEvents();
//        //QThread::currentThread()->msleep(300);
//        //QThread::currentThread()->yieldCurrentThread();    //不放弃当前线程，让它执行myFinished
//     }

//     if (pUpdate->bDownloaded)
//         qDebug() << " 已下载";

    //0. 初始化
    //0.1 寻找系统 ini  //my documents/\xjx_jxs/config.ini
    QStringList slist = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QDir documentsDir = slist.at(0);
    QString configIni = "/xjx_jxs/config.ini";
    QString configIniWhole = documentsDir.path() + configIni;

    //0.2 读入INI
    QSettings configIniRead(configIniWhole, QSettings::IniFormat);
    //0.2.1 config
    curUserName = configIniRead.value("/program/username").toString();
    if (curUserName.isEmpty()) isLogin = false;
    else isLogin = true;
    curUserZone = configIniRead.value("/program/userZone").toDouble();
    curVersion = configIniRead.value("/program/version").toDouble();
    curInstalledDir = configIniRead.value("/program/installedDir").toString();

    QSettings configIniWrite(configIniWhole, QSettings::IniFormat);
    configIniWrite.setValue("/program/installedDir", QDir::currentPath().trimmed());

    //0.2.2 update
    baseUrl = configIniRead.value("/update/baseUrl").toString();
    tmpExeFileName = configIniRead.value("/update/exeFileName").toString();
    versionFileName = configIniRead.value("/update/versionFileName").toString();
    updateDir = configIniRead.value("/update/updateLocalDir").toString();

    isThereNewUpdate = false;   //先设false，后面有比较版本号，如果大于当前版本，则被设true
    qDebug() << "---" <<curVersion << "==" <<curUserName << "==" <<isLogin;

    // 1.托盘图标
    // 1.1 初始化图标
    iconCorrect = QIcon(":/images/correct.png");
    iconError = QIcon(":/images/error.png");

    // 1.2 初始化托盘图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(tr("单击显示系统信息\n右击显示功能菜单"));
    setBothIcons(0);
    trayIcon->setVisible(true);

    // 显示托盘图标功能菜单
    setupTrayMenu();

    // 实现托盘图标左击功能
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(myIconActivated(QSystemTrayIcon::ActivationReason)));

    //2. 读入升级程序
    //2.1 网络下载
    UpdateByNetwork * pUpdateVerFile = new UpdateByNetwork();
    pUpdateVerFile->setBaseAddress(baseUrl);
    pUpdateVerFile->setDownloadFileName(versionFileName);
    pUpdateVerFile->setlocalUpdateDir(updateDir);
    pUpdateVerFile->startDownload();
    while (!pUpdateVerFile->isDownloaded()) {      //法2：可以等待下载线程完成下载。
        QCoreApplication::processEvents();
       //QThread::currentThread()->msleep(300);
       //QThread::currentThread()->yieldCurrentThread();    //不放弃当前线程，让它执行myFinished
    }

    //2.2 打开文件
    QString filename = updateDir + "/" + versionFileName;
    QFile file(filename);
    if (file.size() == 0) {
        QMessageBox::information(this, "升级", "升级文件检测失败，请检查您的网络是否正常。");
    } else {
        //打开文件，读版本号
        QSettings updateIniRead(filename, QSettings::IniFormat);
        QString updateZone = updateIniRead.value("/update/updateZone").toString();
        double updateVersion = updateIniRead.value("/update/updateVersion").toDouble();
        QString strUpdateVersion = updateIniRead.value("/update/updateVersion").toString();
        //qDebug() << "here" << updateVersion << updateZone << curUserZone;
        if (!curUserZone.isEmpty()
                && (updateZone.contains(curUserZone) || updateZone.contains("ALL"))
                && (updateVersion > curVersion)) { //同一区域，版本号不同
            isThereNewUpdate = true;    //不写INI文件，升级后由升级程序直接拷贝my documents/config.ini
            QString tmpZone = updateZone.contains("ALL")? "ALL" : curUserZone;
            exeFileName = tmpZone + tmpExeFileName + strUpdateVersion + ".exe";
        }
    }
    file.remove();

    if (isThereNewUpdate) {
        ui->label_3->setText("发现新版本，请点击升级程序按钮，否则无法正常使用该程序。");
        ui->pushButton->setEnabled(false);
        ui->progressBar->hide();
    } else {
        ui->label_3->setText("未发现新版本程序。");
        ui->pushButton_2->setEnabled(false);
        ui->progressBar->hide();
    }

}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::isLoginFunc()
{
    return isLogin;
}

bool Dialog::isThereNewUpdateFunc()
{
    return isThereNewUpdate;
}

// 升级相关
void Dialog::on_pushButton_2_clicked()
{
    //显示进度条
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    ui->label_3->setText("正在下载升级程序，请稍候...");

    //升级程序
    UpdateByNetwork * pUpdateExeFile = new UpdateByNetwork();
    pUpdateExeFile->setBaseAddress(baseUrl);
    pUpdateExeFile->setDownloadFileName(exeFileName);
    pUpdateExeFile->setlocalUpdateDir(updateDir);
    pUpdateExeFile->startDownload();
    connect(pUpdateExeFile->pReply, QNetworkReply::downloadProgress, this, myDownloadProgress);
    while (!pUpdateExeFile->isDownloaded()) {      //法2：可以等待下载线程完成下载。
        QCoreApplication::processEvents();
       //QThread::currentThread()->msleep(300);
       //QThread::currentThread()->yieldCurrentThread();    //不放弃当前线程，让它执行myFinished
    }

    QString exe = updateDir + "/" + exeFileName;
    QFile exeFile(exe);
    if (!exeFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "升级", "下载升级程序失败，请检查网络是否连通。");
        exeFile.close();
        delete pUpdateExeFile;
        return;
    } else if (exeFile.size() < 4096){
        if (exeFile.size() == 0)    //未连接网络，下载的程序大小为0
            QMessageBox::information(this, "升级", "下载升级程序失败，请检查网络是否连通。");
        if (exeFile.size() < 4096)  //连接了网络，但下载源错误，下载了2K文件（内容为tomcat错误代码）
            QMessageBox::information(this, "升级", "下载升级程序失败，下载源不存在。请联系客服。");
        exeFile.close();
        delete pUpdateExeFile;
        return;
    }

    //已下载：执行升级程序，关闭当前程序
    qDebug() << " EXE已下载";
    delete pUpdateExeFile;
    ui->label_3->setText("升级程序下载完毕。");
    ui->progressBar->hide();
    ui->pushButton_2->setEnabled(false);
    QMessageBox::information(this, "升级", "下载升级程序成功，点击确定开始升级。\n\n点击确定后，将关闭当前程序并启动升级程序。");
    QProcess::startDetached(updateDir + "/" + exeFileName);
    //QProcess::startDetached("update/xjx_jxs_setup.exe");

    qApp->quit();
}

void Dialog::myDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
}

//1. 窗口最小化到托盘
//1.1 设置图标
void Dialog::setBothIcons(int index)
{
    // index = 0; 无错误
    // index = 1; 有错误,显示错误图标
    (index == 0) ? setWindowIcon(iconCorrect) : setWindowIcon(iconError);
    (index == 0) ? trayIcon->setIcon(iconCorrect) : trayIcon->setIcon(iconError);
}

//1.2 设置菜单
void Dialog::setupTrayMenu()
{
    // 要检索INI和设备信息后才处理
    userMsg = new QAction(tr("当前用户："), this);
    deviceMsg = new QAction(tr("当前设备："), this);
    logoutAction = new QAction(tr("注销用户"), this);
    quitAction = new QAction(tr("退出程序"), this);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(userMsg);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(deviceMsg);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction(logoutAction);
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));

    trayIconMenu->addAction(quitAction);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));     // 非常重要,必须是qApp,不能是this

    trayIcon->setContextMenu(trayIconMenu);
}

//1.3 处理close事件，不关闭系统，而是最小化到托盘
void Dialog::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible())
    {
        if(isLogin)
            hide();
        else
        {
            QMessageBox::information(this, tr("远程传输与控制系统"), tr("请登录。\n\n退出程序：请右击系统托盘图标，选择退出程序"));
        }
        event->ignore();
    }
}

//1.4 点击托盘图标产生事件
void Dialog::myIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showMessage();
        break;
    default:
        break;
    }
}

//1.4.1 showMessage //显示托盘图标消息  //左键
void Dialog::showMessage()
{
    deviceInfo.clear();
    QString title = tr("系统状态信息");
    deviceInfo.append(tr("1. aaa"));
    deviceInfo.append(tr("2. bbb"));
    int duration = 3;
    trayIcon->showMessage(title, deviceInfo.join("\n"),     // bodyEdit->toPlainText()
                          QSystemTrayIcon::Information,     // icon
                          duration * 1000);
}

//2. 登录相关
//2.1 用户登录
void Dialog::on_pushButton_clicked()
{
    //1. 取出用户/密码

    //2. 使用hessian登录

    //3. 如登录失败，返回错误信息
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::information(this, "登录", "登录失败，请输入正确用户名与密码。");
        return;
    }

    //4. 登录成功，最小化到托盘（伪调用该函数）
    isLogin = true;
    //给userName=
    //写INI文件
    hide();
}

//2.2 注销
void Dialog::logout()
{
    isLogin = false;
    showNormal();
}

