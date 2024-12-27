<!-- 使用qt实现支持断点续传的下载类 -->

IDownloadHelper.h：

```c++
#ifndef _IDOWNLOADHELPER_H
#define _IDOWNLOADHELPER_H

#include <QVariant>

class IDownloadHelper {
public:
    enum NotifyType {
        NotifyType_finish,
        NotifyType_progress,
        NotifyType_error
    };

    enum DownloaderType {
        DownloaderType_qt
    };

    enum ExecType {
        ExecType_block,
        ExecType_timeout
    };

    using DownloadNotifyFun = std::function<void(NotifyType type, const QVariant &)>;

    void asyncStart(const QString &url, const QString &localPath,
                    const IDownloadHelper::DownloadNotifyFun &callback = nullptr);

    virtual void
    doAsyncStart(const QString &url, const QString &localPath, const IDownloadHelper::DownloadNotifyFun &callback) = 0;

    virtual void stop() = 0;

    virtual void pause() = 0;

    virtual void resume() = 0;

    virtual bool checkMd5(const QString &filePath, const QString &desMd5) = 0;

    virtual bool exec(ExecType type = ExecType_block, int timeoutSec = 0) = 0;

    static std::shared_ptr<IDownloadHelper> create(DownloaderType type = DownloaderType_qt);


protected:

    IDownloadHelper();

    virtual ~IDownloadHelper() {}

    QString getDownloadUrl();

    QString getLocalFilePath();

    void calDownloadNotifyFun(NotifyType type, const QVariant &data);

private:
    DownloadNotifyFun _callback;
    QString _url;
    QString _localFilePath;

};

#endif //_IDOWNLOADHELPER_H

```

IDownloadHelper.cpp:

```c++
#include "IDownloadHelper.h"
#include "DownloadHelperQt.h"
#include <QDebug>

QString IDownloadHelper::getDownloadUrl() {
    return _url;
}

void IDownloadHelper::calDownloadNotifyFun(IDownloadHelper::NotifyType type, const QVariant &data) {
    if (_callback) {
        _callback(type, data);
    }
}

std::shared_ptr<IDownloadHelper> IDownloadHelper::create(IDownloadHelper::DownloaderType type) {
    std::shared_ptr<IDownloadHelper> ret;
    if (IDownloadHelper::DownloaderType_qt == type) {
        ret = std::make_shared<DownloadHelperQt>();
    }
    return ret;
}

void IDownloadHelper::asyncStart(const QString &url, const QString &localPath,
                                 const IDownloadHelper::DownloadNotifyFun &callback) {
    _callback = callback;
    qDebug() << "asyncStart network ,url is " << url;
    _localFilePath = localPath;
    doAsyncStart(url, localPath, callback);
}

IDownloadHelper::IDownloadHelper() {

}

QString IDownloadHelper::getLocalFilePath() {
    return _localFilePath;
}
```

DownloadHelperQt.h

```c++

#ifndef _DOWNLOADHELPERQT_H
#define _DOWNLOADHELPERQT_H

#include <QPointer>
#include "IDownloadHelper.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class DownloadHelperQt : public QObject,public IDownloadHelper {
    Q_OBJECT
public:
    DownloadHelperQt();

    void doAsyncStart(const QString &url, const QString &localPath,
                      const IDownloadHelper::DownloadNotifyFun &callback) override;

    void stop() override;

    void pause() override;

    void resume() override;

    bool checkMd5(const QString &filePath, const QString &desMd5) override;

    bool exec(ExecType type, int timeoutSec) override;

    signals:
    void downloadComplete(bool ok);

private:
    QNetworkAccessManager _netManager;
    QNetworkReply *_reply = nullptr;
    QNetworkRequest _curRequest;
    QFile _file;
    QString _tmpFileName{"download.part"};
    bool _complete{false};
    bool _done{false};

    quint64  _downloadSizeAtPause = 0;

    void completed(bool ok);

    void download(QNetworkRequest &request);
};


#endif //_DOWNLOADHELPERQT_H
```

DownloadHelperQt.cpp:

```c++

#include "DownloadHelperQt.h"
#include <QEventLoop>
#include <QFile>
#include <QTimer>
#include <thread>
#include <QCoreApplication>

void DownloadHelperQt::doAsyncStart(const QString &url, const QString &localPath,
                                    const IDownloadHelper::DownloadNotifyFun &callback) {
    _done = false;
    _complete = false;
    _downloadSizeAtPause = 0;
    _curRequest = QNetworkRequest(QUrl(url));
    QFile::remove(_tmpFileName);
    _file.setFileName(_tmpFileName);
    _file.open(QIODevice::ReadWrite);
    download(_curRequest);
}

void DownloadHelperQt::download(QNetworkRequest &request) {
    _reply = _netManager.get(request);
    if (_reply) {
        connect(_reply, &QNetworkReply::downloadProgress, this, [this](qint64 bytesReceived, qint64 bytesTotal) {
            calDownloadNotifyFun(NotifyType_progress, bytesReceived * 1.0 / bytesTotal);
        });
    }
}

void DownloadHelperQt::stop() {
    if (_reply) {
        _reply->abort();
        completed(false);
    }
}

void DownloadHelperQt::pause() {
    if (_reply) {
        disconnect(_reply, &QNetworkReply::downloadProgress, this, nullptr);
        _downloadSizeAtPause = _reply->bytesAvailable();
        qDebug() << "downloadSizeAtPause" << _downloadSizeAtPause;
        _file.write(_reply->readAll());
        _file.close();
        _reply->abort();
        _reply = nullptr;
    }
}

void DownloadHelperQt::resume() {
    _downloadSizeAtPause = _file.size();
    QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(_downloadSizeAtPause) + "-";
    qDebug() << "rangeHeaderValue" << rangeHeaderValue;
    _curRequest.setRawHeader("Range", rangeHeaderValue);
    download(_curRequest);
}

bool DownloadHelperQt::exec(ExecType type, int timeoutSec) {
    bool ret = false;
    do {
        if (type == ExecType_timeout) {
            QTimer::singleShot(timeoutSec * 1000, this, [this]() {
                stop();
            });
        }

        while (!_done) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            qApp->processEvents();
        }
        ret = _complete;
    } while (false);
    return ret;
}

DownloadHelperQt::DownloadHelperQt() : IDownloadHelper() {

    connect(&_netManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (nullptr == reply) {
            completed(false);
            return;
        }
        qInfo() << "reply->error() is " << reply->error();
        if (reply->error() == QNetworkReply::NoError) {
            // Handle successful completion
            QFile file(getLocalFilePath());
            if (file.open(QIODevice::WriteOnly)) {
                _file.open(QIODevice::ReadOnly);
                if (_downloadSizeAtPause > 0) {
                    // If the download was paused, write the data to the file
                    file.write(_file.readAll());
                }
                file.write(reply->readAll());
                file.close();
            } else {
                qDebug() << "Error opening file for writing.";
            }

            completed(true);
        } else if (QNetworkReply::OperationCanceledError == reply->error()) {
            qDebug() << "Download canceled.";
        } else {
            // Handle error
            completed(false);
            calDownloadNotifyFun(IDownloadHelper::NotifyType_error, reply->errorString());
        }
    });

    connect(&_netManager, &QNetworkAccessManager::sslErrors, this,
            [this](QNetworkReply *reply, const QList<QSslError> &errors) {
                calDownloadNotifyFun(IDownloadHelper::NotifyType_error, reply->errorString());
                completed(false);
            });

}

void DownloadHelperQt::completed(bool ok) {
    calDownloadNotifyFun(IDownloadHelper::NotifyType_finish, ok);
    emit downloadComplete(ok);
    _file.close();
    QFile::remove(_tmpFileName);
    _complete = ok;
    _done = true;
}

bool DownloadHelperQt::checkMd5(const QString &filePath, const QString &desMd5) {
    bool ret = false;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading:" << file.errorString();
        return ret;
    }
    qint64 fileSize = file.size();
    const qint64 BUFFER_SIZE = 10240;

    char buffer[BUFFER_SIZE] = {0};
    int bytesRead = 0;
    int readSize = qMin(fileSize, BUFFER_SIZE);

    QCryptographicHash hash(QCryptographicHash::Md5);
    while (readSize > 0 && (bytesRead = file.read(buffer, readSize)) > 0) {
        fileSize -= bytesRead;
        hash.addData(buffer, bytesRead);
        readSize = qMin(fileSize, BUFFER_SIZE);
    }

    QByteArray md5Hash = hash.result().toHex();
    qDebug() << "checkMd5 origin md5 is " << md5Hash;
    ret = (QString(md5Hash) == desMd5);
    return ret;
}
```

test.cpp:

```c++
#ifndef CATCH2TEST_DOWNLOADHELPERTEST_CPP
#define CATCH2TEST_DOWNLOADHELPERTEST_CPP

#include "../catch_amalgamated.hpp"
#include "IDownloadHelper.h"
#include <QObject>
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

TEST_CASE("downloder", "[1]") {
    int i = 1;
    QCoreApplication app(i, nullptr);
    auto helper = IDownloadHelper::create(IDownloadHelper::DownloaderType_qt);
    const QString downloadUrl = "todoUrl";
    const QString desPath = "todoUrl.zip";
    const QString desMd5Sum = "todoMd5";

    REQUIRE(helper);

    SECTION("download ok") {
        helper->asyncStart(downloadUrl,desPath ,
                           [](IDownloadHelper::NotifyType type, const QVariant &data) {
                               if (IDownloadHelper::NotifyType_finish == type) {
                                   std::cout << "finished ,and result is " << data.toBool() << std::endl;
                               } else if (IDownloadHelper::NotifyType_error == type) {
                                   std::cout << "error ,and error info is " << data.toString().toStdString()
                                             << std::endl;
                               } else if (IDownloadHelper::NotifyType_progress == type) {
                                   std::cout << "progress ,and progress is " << data.toFloat() << std::endl;
                               }
                           });

        QTimer::singleShot(1000, qApp, [&]() {
            std::cout << "downloadHelper pause" << std::endl;
            helper->pause();
        });

        QTimer::singleShot(2000, qApp, [&]() {
            std::cout << "downloadHelper resume" << std::endl;
            helper->resume();
        });

        REQUIRE(helper->exec());
        REQUIRE(helper->checkMd5(desPath,desMd5Sum));
    }

    SECTION("download stop") {
        helper->asyncStart(downloadUrl, desPath,
                           [](IDownloadHelper::NotifyType type, const QVariant &data) {
                               if (IDownloadHelper::NotifyType_finish == type) {
                                   std::cout << "finished ,and result is " << data.toBool() << std::endl;
                               } else if (IDownloadHelper::NotifyType_error == type) {
                                   std::cout << "error ,and error info is " << data.toString().toStdString()
                                             << std::endl;
                               } else if (IDownloadHelper::NotifyType_progress == type) {
                                   std::cout << "progress ,and progress is " << data.toFloat() << std::endl;
                               }
                           });

        REQUIRE(!helper->exec(IDownloadHelper::ExecType_timeout, 1));
    }

}

#endif //CATCH2TEST_DOWNLOADHELPERTEST_CPP
```

CMakeLists.txt:

```cmake

find_package(
        Qt5
        CONFIG
        REQUIRED
        Core
        Network
)
set(CMAKE_AUTOMOC ON)

set(DOWNLOADER_FILES
        IDownloadHelper.h
        IDownloadHelper.cpp
        DownloadHelperQt.h
        DownloadHelperQt.cpp
)

set(CATCH2_FILES ../catch_amalgamated.hpp ../catch_amalgamated.cpp)
add_executable(testDownloader downloadHelperTest.cpp ${DOWNLOADER_FILES} ${CATCH2_FILES})

if (WIN32)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libcrypto-1_1.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libssl-1_1.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif (WIN32)

target_link_libraries(
        testDownloader
        Qt5::Network
        Qt5::Core
)
```

- [上一级](README.md)
- 上一篇 -> [解决 qt 程序的依赖项](deployqt.md)
- 下一篇 -> [Qt Creator 的一些配置](projectConfig.md)
