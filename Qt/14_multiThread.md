# 14.多线程


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [14.多线程](#14多线程)
	* [创建线程](#创建线程)
	* [参考链接](#参考链接)

<!-- /code_chunk_output -->
## 创建线程
[QThread Class](http://doc.qt.io/qt-5/qthread.html)

在 Qt 应用程序中提供多线程是非常简单的：只需要子类化 QThread 并且重新实现它的 run() 函数就可以了。可参考如下示例：
```c++
#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread();

    void setMessage(const QString &message);
    void stop();

protected:
    void run() override;

private:
    QString messageStr;
    volatile bool stopped;
    //volatile 拒绝编译器优化
};

#endif
```

```c++
#include <QtCore>
#include <iostream>

#include "thread.h"

Thread::Thread()
{
    stopped = false;
}

void Thread::setMessage(const QString &message)
{
    messageStr = message;
}

void Thread::run()
{
    while (!stopped)
        std::cerr << qPrintable(messageStr);
    stopped = false;
    std::cerr << std::endl;
}

void Thread::stop()
{
    stopped = true;
}
```
在开始执行线程的时候，就会调用 run() 函数。

QThread 提供了一个 terminate() 函数，该函数可以在一个线程还在运行的时候就终止它的执行。我们不推荐使用 terminate() 方法，这是因为它可以随时停止线程执行而不给这个线程自我清空的机会。一种更为安全的方法是使用 stopped 变量和stop() 函数，就像这里所做的那样。

```c++
#ifndef THREADDIALOG_H
#define THREADDIALOG_H

#include <QDialog>

#include "thread.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class ThreadDialog : public QDialog
{
    Q_OBJECT

public:
    ThreadDialog(QWidget * parent = 0);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void startOrStopThreadA();
    void startOrStopThreadB();

private:
    Thread threadA;
    Thread threadB;
    QPushButton * threadAButton;
    QPushButton * threadBButton;
    QPushButton * quitButton;
};

#endif
```
```c++
#include <QtWidgets>

#include "threaddialog.h"

ThreadDialog::ThreadDialog(QWidget *parent)
    : QDialog(parent)
{
    threadA.setMessage("A");
    threadB.setMessage("B");

    threadAButton = new QPushButton(tr("Start A"));
    threadBButton = new QPushButton(tr("Start B"));
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setDefault(true);

    connect(threadAButton, SIGNAL(clicked()),
            this, SLOT(startOrStopThreadA()));
    connect(threadBButton, SIGNAL(clicked()),
            this, SLOT(startOrStopThreadB()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addWidget(threadAButton);
    mainLayout->addWidget(threadBButton);
    mainLayout->addWidget(quitButton);
    setLayout(mainLayout);

    setWindowTitle(tr("Threads"));
}

void ThreadDialog::startOrStopThreadA()
{
    if (threadA.isRunning()) {
        threadA.stop();
        threadAButton->setText(tr("Start A"));
    } else {
        threadA.start(); // will call run() function
        threadAButton->setText(tr("Stop A"));
    }
}

void ThreadDialog::startOrStopThreadB()
{
    if (threadB.isRunning()) {
        threadB.stop();
        threadBButton->setText(tr("Start B"));
    } else {
        threadB.start(); // will call run() function
        threadBButton->setText(tr("Stop B"));
    }
}

void ThreadDialog::closeEvent(QCloseEvent *event)
{
    threadA.stop();
    threadB.stop();
    threadA.wait();
    threadB.wait();
    event->accept();
}
```

## 参考链接
* [Threading Basics](http://doc.qt.io/qt-5/thread-basics.html)
[上一级](base.md)
[上一篇](2_creat_dialog.md)
