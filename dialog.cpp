#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // 1.托盘图标
    // 1.1 初始化图标
    iconCorrect = QIcon(":/images/correct.png");
    iconError = QIcon(":/images/error.png");

    // 1.2 初始化托盘图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(tr("单击显示系统信息\n右击显示功能菜单"));
    setBothIcons(0);
    trayIcon->setVisible(true);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    // 1.判断用户/密码是否为空并获取相应的值
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::information(this, tr("登录"), tr("用户名与密码不能为空"));
        return;
    }

    ui->pushButton->setEnabled(false);
    // 2.使用Hessian登录
    // 3.其他内容
    isLogin = false;

    ui->pushButton->setEnabled(true);
}

void Dialog::on_pushButton_2_clicked()
{

}

void Dialog::setBothIcons(int index)
{
    // index = 0; 无错误
    // index = 1; 有错误,显示错误图标
    (index == 0) ? setWindowIcon(iconCorrect) : setWindowIcon(iconError);
    (index == 0) ? trayIcon->setIcon(iconCorrect) : trayIcon->setIcon(iconError);
}
