#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
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
