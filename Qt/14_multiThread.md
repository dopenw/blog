# 14.多线程


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [14.多线程](#14多线程)
	* [创建线程](#创建线程)
	* [同步线程](#同步线程)
		* [生产者-消费者](#生产者-消费者)
			* [使用信号量](#使用信号量)
			* [使用条件变量](#使用条件变量)
		* [线程本地存储](#线程本地存储)
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
## 同步线程
对于多线程应用程序，一个最基本的要求就是能实现几个线程的同步执行。Qt 提供了以下用于同步的类： [QMutex](http://doc.qt.io/qt-5/qmutex.html),[QReadWriteLock](http://doc.qt.io/qt-5/qreadwritelock.html),[QSemaphore](http://doc.qt.io/archives/qt-4.8/qsemaphore.html) 和 [QWaitCondition](http://doc.qt.io/archives/qt-4.8/qwaitcondition.html).

Qt 提供了方便的 [QMutexLocker](http://doc.qt.io/qt-5/qmutexlocker.html) 类来简化对互斥量的处理。类似于 [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard).

QSemaphore 是互斥量的另外一种泛化表示形式，但与 读-写 锁定不同，信号量可以用于保护一定数量的相同资源。下面两小段程序代码给出了 QSemaphore 和 QMutex 之间的对应关系：
```c++
QSemaphore semaphore(1);
semaphore.acquire();
semaphore.release();
```
```c++
QMutex mutex;
mutex.lock();
mutex.unlock();
```

### 生产者-消费者
#### 使用信号量
```c++
#include <QtCore>
#include <iostream>

const int DataSize = 100000;
const int BufferSize = 4096;
char buffer[BufferSize];

QSemaphore freeSpace(BufferSize);
QSemaphore usedSpace(0);

class Producer : public QThread
{
public:
    void run();
};

void Producer::run()
{
    for (int i = 0; i < DataSize; ++i) {
        freeSpace.acquire();
        buffer[i % BufferSize] = "ACGT"[uint(std::rand()) % 4];
        usedSpace.release();
    }
}

class Consumer : public QThread
{
public:
    void run();
};

void Consumer::run()
{
    for (int i = 0; i < DataSize; ++i) {
        usedSpace.acquire();
        std::cerr << buffer[i % BufferSize];
        freeSpace.release();
    }
    std::cerr << std::endl;
}

int main()
{
    Producer producer;
    Consumer consumer;
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
    return 0;
}
```
#### 使用条件变量
```c++
#include <QtCore>
#include <iostream>

const int DataSize = 100000;
const int BufferSize = 4096;
char buffer[BufferSize];

QWaitCondition bufferIsNotFull;
QWaitCondition bufferIsNotEmpty;
QMutex mutex;
int usedSpace = 0;

class Producer : public QThread
{
public:
    void run();
};

void Producer::run()
{
    for (int i = 0; i < DataSize; ++i) {
        mutex.lock();
        while (usedSpace == BufferSize)
            bufferIsNotFull.wait(&mutex);
        buffer[i % BufferSize] = "ACGT"[uint(std::rand()) % 4];
        ++usedSpace;
        bufferIsNotEmpty.wakeAll();
        mutex.unlock();
    }
}

class Consumer : public QThread
{
public:
    void run();
};

void Consumer::run()
{
    for (int i = 0; i < DataSize; ++i) {
        mutex.lock();
        while (usedSpace == 0)
            bufferIsNotEmpty.wait(&mutex);
        std::cerr << buffer[i % BufferSize];
        --usedSpace;
        bufferIsNotFull.wakeAll();
        mutex.unlock();
    }
    std::cerr << std::endl;
}

int main()
{
    Producer producer;
    Consumer consumer;
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
    return 0;
}
```

### 线程本地存储
在不同线程中保存不同数值的全局变量。这种变量通常称为 线程本地存储 或者 线程特定数据。

[QThreadStorage Class](http://doc.qt.io/qt-5/qthreadstorage.html)
## 参考链接
* [Threading Basics](http://doc.qt.io/qt-5/thread-basics.html)

[上一级](base.md)
[上一篇](2_creat_dialog.md)
