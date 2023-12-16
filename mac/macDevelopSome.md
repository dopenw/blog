<!-- mac 开发资料 -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [利用 cmake 生成 Xcode 工程](#利用-cmake-生成-xcode-工程)
- [利用 qmake 生成 Xcode 工程](#利用-qmake-生成-xcode-工程)
- [其他问题](#其他问题)
  - [fatal error: ‘string.h’ file not found #include_next <string.h>](#fatal-error-stringh-file-not-found-include_next-stringh)
  - [无法下载 appstore 版本的软件](#无法下载-appstore-版本的软件)
  - [Homebrew](#homebrew)
  - [查看崩溃报告](#查看崩溃报告)
  - [重设麦克风/相机等请求权限，解决勾选相关权限，实际没生效的问题](#重设麦克风相机等请求权限解决勾选相关权限实际没生效的问题)
  - [让 Xcode 能够显示qt 类型及标准库的变量值](#让-xcode-能够显示qt-类型及标准库的变量值)
  - [证书问题](#证书问题)
  - [Xcode 如何使用旧版本SDK以保证程序兼容性](#xcode-如何使用旧版本sdk以保证程序兼容性)
- [编译支持生成universal安装包 的QT5](#编译支持生成universal安装包-的qt5)
    - [相关链接](#相关链接)
- [error: function does not return NSString](#error-function-does-not-return-nsstring)
- [改变 xcode 的默认 SDK](#改变-xcode-的默认-sdk)
- [基于notarytool对MacOS应用进行公证](#基于notarytool对macos应用进行公证)
  - [一些实用的开发工具](#一些实用的开发工具)
    - [Git 客户端](#git-客户端)
    - [下载工具](#下载工具)
  - [Link](#link)

<!-- /code_chunk_output -->



# 利用 cmake 生成 Xcode 工程

- 使用 qt creator 创建 cmake 工程
- 使用 cmake gui ，选择where is the source code ,where to build the binaries 目录，点击 File → Delete cache,点击 Configure 按钮，设置目标工程为 Xcode 后关闭，点击 Generate 按钮开始转换工程

- 若提示找不到 QT_DIR 则添加对应的 qt 目录，eg - /Users/admin/Qt/5.15.2/clang_64/lib/cmake/Qt5
- 打开生成的 xcode 工程文件，编译运行即可

# 利用 qmake 生成 Xcode 工程

- 使用 qt creator 创建 qmake 工程
- 执行下面的命令即可生成对应的工程,比如

```bash
/Users/admin/Qt/5.15.2/clang_64/bin/qmake -spec macx-xcode untitled5.pro
```

运行命令后，出现该警告，需注意：
```sh
Info: creating stash file /Users/admin/Documents/test/untitled5/.qmake.stash

Project WARNING: Qt has only been tested with version 10.15 of the platform SDK, you're using 11.1.

Project WARNING: This is an unsupported configuration. You may experience build issues, and by using

Project WARNING: the 11.1 SDK you are opting in to new features that Qt has not been prepared for.

Project WARNING: Please downgrade the SDK you use to build your app to version 10.15, or configure

Project WARNING: with CONFIG+=sdk_no_version_check when running qmake to silence this warning.
```

- 打开生成的 xcode 工程文件，编译运行即可


# 其他问题

## fatal error: ‘string.h’ file not found #include_next <string.h>

解决方法：删除工程下的 .qmake.stash 然后重新编译即可

[Qt Creator for Mac：升级Xcode导致工程编译出错 - 代码先锋网](https://www.codeleading.com/article/94993555233/)

## 无法下载 appstore 版本的软件

在 Console → Log reporter → install.log 中找到了原因（是本机安装了高版本的软件造成不能下载该软件，oops） 

## Homebrew

Homebrew是什么？它是Mac上的包管理器，类似CentOs的yum等，让你下载、管理一条命令搞定，不需要每次去网上搜索、下载，具体可以看官网介绍 [Homebrew官网](https://brew.sh/index_zh-cn)

- 使用国内源安装 ，可参考

[homebrew | 镜像站使用帮助 | 清华大学开源软件镜像站 | Tsinghua Open Source Mirror](https://mirror.tuna.tsinghua.edu.cn/help/homebrew/)

## 查看崩溃报告

找到 控制台 （应用程序），→ 崩溃报告

[扒虫篇－崩溃日志解读及Crash收集](https://cloud.tencent.com/developer/article/1336404)

## 重设麦克风/相机等请求权限，解决勾选相关权限，实际没生效的问题

```bash
tccutil reset Camera;
tccutil reset Microphone;
tccutil reset ScreenCapture;
```

[How do I reset Camera and Microphone permission on macOS Mojave?](https://apple.stackexchange.com/questions/384317/how-do-i-reset-camera-and-microphone-permission-on-macos-mojave)

## 让 Xcode 能够显示qt 类型及标准库的变量值

1. `pip3 install pygdbmi`
2. cp -a “～/Qt/Qt [Creator.app/Contents/Resources/debugger](http://Creator.app/Contents/Resources/debugger)“ ~/
3. 修改 ～/debugger/[lldbbridge.py](http://lldbbridge.py/) 文件
    
    ```python
    #让脚本找到 pygdbmi 相关的库
    sys.path.append(r"/opt/homebrew/lib/python3.9/site-packages")
    
    if encoding in text_encodings:
                     try:
    #修改下面的代码
    -                    binaryvalue = summaryValue.decode('hex')
    #变更为
    +                    binaryvalue = bytearray.fromhex(summaryValue)
                         # LLDB expects UTF-8
                         return "\"%s\"" % (binaryvalue.decode(encoding).encode('utf8'))
                     except:
    ```
    
4. vim ~/.lldbinit
    
    ```bash
    command script import ～/debugger/lldbbridge.py
    ```
    
5. 退出Xcode并重启，打开断点，进入lldb调试模式，执行以下命令：
command source ~/.lldbinit

## 证书问题

[OS X productsign error: Could not find appropriate signing identity](https://stackoverflow.com/questions/21244756/os-x-productsign-error-could-not-find-appropriate-signing-identity)

[3rd Party Mac Developer Application: DEVELOPER NAME: no identity found](https://stackoverflow.com/questions/48274880/3rd-party-mac-developer-application-developer-name-no-identity-found)


在  apple developer 里面创建新的Mac Installer Distribution 和 Mac App Distribution 证书（需要本机向证书颁发机构请求证书。安装其他机器创建证书，安装后也是无效的），安装流程是：

you have to log on to developer.apple.com with your apple ID and

1- try remove and download your certificate again

2- if this is not enough create a new with with the following steps

2a - click on the (+) or go here:

[https://developer.apple.com/account/mac/certificate/distribution/create](https://developer.apple.com/account/mac/certificate/distribution/create)

2b - specify Production / Developer ID, then press Continue

2c - specify "Developer ID Installer"

2d - follow, the instruction to create a certificate request using keychain

2e - upload your certificate request file in apple developer

2f - download the newly created certificate in your keychain.

2g - cleanup all other installer certificate to be sure.

Et voila, you can sign.

[Remove private key from Mac OS X keychain using Terminal](https://stackoverflow.com/questions/7678057/remove-private-key-from-mac-os-x-keychain-using-terminal)

提交后有 **ITMS-90721: Certificate Revoked 問題，可參考**

[XCode insists on using a revoked certificate](https://stackoverflow.com/questions/61127497/xcode-insists-on-using-a-revoked-certificate)

## Xcode 如何使用旧版本SDK以保证程序兼容性

[GitHub - phracker/MacOSX-SDKs: A collection of those pesky SDK folders: MacOSX10.1.5.sdk thru MacOSX11.3.sdk](https://github.com/phracker/MacOSX-SDKs)

[Xcode 如何使用旧版本SDK以保证程序兼容性_SoldierJazz2021的博客-CSDN博客](https://blog.csdn.net/JAZZSOLDIER/article/details/52229203)

改变工程中 Build Setting → Architectures → Base SDK 为指定的 SDK 即可。

# 编译支持生成universal安装包 的QT5

[wqc_wood](https://www.cnblogs.com/wqcwood/p/15138983.html)

### 相关链接

[Debug Qt app with Xcode 11 and DataFormatter](https://stackoverflow.com/questions/60963616/debug-qt-app-with-xcode-11-and-dataformatter)

[Xcode lldb调试器使用](https://www.jianshu.com/p/5f23c2b251aa)

# error: function does not return NSString

在 xcode 里面添加预处理宏 

```cpp
**NS_FORMAT_ARGUMENT(A)=**
```

make 里面就这样：

```cpp
make CXXFLAGS=-D"NS_FORMAT_ARGUMENT\(A\)="
```

原文链接：

[NSBundle.h:91:143: error: function does not return NSString · Issue #69 · sbmpost/AutoRaise](https://github.com/sbmpost/AutoRaise/issues/69)

# 改变 xcode 的默认 SDK

[[XCODE] xcode 同时支持安装多个SDK的方式_池塘码农_党伟的博客-CSDN博客](https://blog.csdn.net/dangwei_90/article/details/110224909)


# 基于notarytool对MacOS应用进行公证

[基于notarytool对MacOS应用进行公证-腾讯云开发者社区-腾讯云](https://cloud.tencent.com/developer/article/2168099)

## 一些实用的开发工具

### Git 客户端

[10 Best GUI Git Clients for Mac](https://www.fossmint.com/gui-git-clients-for-mac/)

### 下载工具

[subhra74/xdm](https://github.com/subhra74/xdm)

## Link

[玩转Mac电脑(程序员开发)](https://blog.csdn.net/z69183787/article/details/87010890)

[Qt for macOS](https://doc.qt.io/qt-5/macos.html)

[Xcode - Apple Developer](https://developer.apple.com/cn/xcode/)

[[Solved]Using Qt from a Xcode project](https://forum.qt.io/topic/8618/solved-using-qt-from-a-xcode-project)

[How to set up Xcode to work with instead of Qt Creator?](https://stackoverflow.com/questions/18500314/how-to-set-up-xcode-to-work-with-instead-of-qt-creator)

[](https://jingyan.baidu.com/article/5225f26ba3cae7e6fb090847.html)

[flock failed to lock maps file: er... | Apple Developer Forums](https://developer.apple.com/forums/thread/106590)

---

- [上一级](README.md)
- 上一篇 -> [上架 mac 应用程序到苹果商店](app2MacAppStore.md)
- 下一篇 -> [qt and mac](qtAndMac.md)
