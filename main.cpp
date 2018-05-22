#include "dialog.h"
#include <QApplication>

// 实现单例模式相关头文件
#include <QMessageBox>
#include <QSystemSemaphore>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 只运行一次,通过共享内存中的标记实现单例模式
    QSystemSemaphore ss("MyObject", 1, QSystemSemaphore::Open);     // 定义信号量
    ss.acquire();       // 加锁
    QSharedMemory mem("MySystemObject");    // 全局对象名
    if(!mem.create(1))                      // 如果全局对象已存在则退出
    {
        QMessageBox::information(0, QObject::tr("警告"), QObject::tr("程序已经运行,无需再行启动"));
        ss.release();   // 释放锁
        return 0;
    }
    ss.release();       // 释放锁

    Dialog w;
    w.show();

    return a.exec();
}
