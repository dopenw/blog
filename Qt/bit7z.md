<!-- 使用 bit7z 解压缩 -->

# bit7z 简介

[bit7z](https://github.com/rikyoz/bit7z) 是一个为 7-zip 共享库提供了干净、简单的接口的静态库。 Qt 自带的解压功能无法解压大文件，bit7z 和 7-zip 满足稳定高效和跨平台，于是采用了该方案。

# Bit7zAdapter

Bit7zAdapter.h:

```c++
#ifndef _BIT7ZADAPTER_H
#define _BIT7ZADAPTER_H


#include <QString>
#include <memory>
#include <QObject>
#include <QTimer>

class Bit7zAdapter : public QObject {
Q_OBJECT
public:
    Bit7zAdapter();

    ~Bit7zAdapter();

    bool extract(const QString &filepath, const QString &outdir);

    void setStop(bool stop) {
        _stop.store(stop);
    }

signals:

    void notifyProgress(float progress);

    void notifyFailed();

private:
    std::uint64_t _totalSize = 1;
    std::atomic<bool> _stop{false};
};


#endif //_BIT7ZADAPTER_H

```



Bit7zAdapter.cpp:

```c++

#include <QFileInfo>
#include "Bit7zAdapter.h"
#include <QDebug>
#include <QGuiApplication>
#include <bit7z/bit7zlibrary.hpp>
#include <bit7z/bitarchivereader.hpp>
#include <bit7z/bitfileextractor.hpp>

using namespace bit7z;

Bit7zAdapter::Bit7zAdapter() {
}

Bit7zAdapter::~Bit7zAdapter() {

}

bool Bit7zAdapter::extract(const QString &filepath, const QString &outdir) {
    bool ret = false;
    try {
        _stop.store(false);
        bit7z::Bit7zLibrary bit7z("7z.dll");
        std::shared_ptr<bit7z::BitFileExtractor> extractor;
        auto fileInfo = QFileInfo(filepath);
        auto suffix = fileInfo.suffix();
        _totalSize = 0;
        if (suffix == "7z") {
            extractor.reset(new BitFileExtractor(bit7z, BitFormat::SevenZip));
        } else if (suffix == "zip") {
            extractor.reset(new BitFileExtractor(bit7z, BitFormat::Zip));
        } else if (suffix == "rar") {
            extractor.reset(new BitFileExtractor(bit7z, BitFormat::Rar));
        } else if (suffix == "tar") {
            extractor.reset(new BitFileExtractor(bit7z, BitFormat::Tar));
        } else if (suffix == "gz") {
            extractor.reset(new BitFileExtractor(bit7z, BitFormat::GZip));
        }

        if (extractor) {
            extractor->setProgressCallback([this](uint64_t size) -> bool {
                if (_totalSize > 0) {
                    auto progress = size * 1.0 / _totalSize;
//                    qDebug() << "_extractor progress current size is : " << size << ", progress = " << progress;
                    emit notifyProgress(progress);
                }
                if (_stop.load()) {
                    return false;
                }
                return true;
            });
            extractor->setTotalCallback([this](uint64_t size) {
                qDebug() << "_extractor total is : " << size;
                _totalSize = size;
            });
//            _extractor->setFileCallback([](bit7z::tstring filepath) {
//                qDebug() << "_extractor file is : " << QString::fromStdString(filepath);
//            });
            extractor->extract(filepath.toStdString(), outdir.toStdString());
            ret = _stop.load() ? false : true;
        }

    } catch (const bit7z::BitException &e) {
        qWarning() << "catch Bit7z Exception - " << e.what();
        if(!_stop.load()) {
            emit notifyFailed();
        }
    } catch (const std::exception &e) {
        qWarning() << "catch std Exception - " << e.what();
        emit notifyFailed();
    }
    return ret;
}
```

---

- [上一级](README.md)
- 上一篇 -> [Qt Creator 使用](QtCreatorTips.md)
- 下一篇 -> [《C++ Qt 设计模式（第二版）》 读书笔记](cppQtDesignPatternsBook.md)
