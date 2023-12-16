<!-- qt and mac -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

  - [查找界面窗口的区域](#查找界面窗口的区域)
  - [获取硬件设备变更的消息](#获取硬件设备变更的消息)
- [关机，睡眠，自启动](#关机睡眠自启动)
- [其它问题](#其它问题)
- [使用 native app Delegate](#使用-native-app-delegate)
  - [示例代码](#示例代码)
  - [动态获取用户是否允许相关权限：](#动态获取用户是否允许相关权限)
  - [MAC 打包 PKG](#mac-打包-pkg)
- [在 mac 上 编译 QT 5.15.2，让其能够生成 universal 程序](#在-mac-上-编译-qt-5152让其能够生成-universal-程序)
  - [编译生成qt版本（需要在 arm 设备和 x86 设备上分别编译）：](#编译生成qt版本需要在-arm-设备和-x86-设备上分别编译)
  - [合并两个版本，制作 universal 版本 Qt](#合并两个版本制作-universal-版本-qt)
  - [编译 universal 程序](#编译-universal-程序)
- [签名编译好的 qt 相关库，要不在其他电脑上使用该版本会有提示无法验证的问题](#签名编译好的-qt-相关库要不在其他电脑上使用该版本会有提示无法验证的问题)
  - [安装 universal  版本 qt creator](#安装-universal--版本-qt-creator)
  - [Qt creator 中添加新的 kits](#qt-creator-中添加新的-kits)
  - [使用 install_name_tool 后，一些库的签名失效。运行 程序加载动态库崩溃](#使用-install_name_tool-后一些库的签名失效运行-程序加载动态库崩溃)
  - [查看签名相关](#查看签名相关)
  - [相关链接](#相关链接)

<!-- /code_chunk_output -->



## 查找界面窗口的区域

```objectivec
QRect CursorWindowRect_mac::FindMainWindow(const QPoint &curMousePos, QString &appName)
{
    auto curPos = QCursor::pos();
    @autoreleasepool {
    // Get all the windows
    CFArrayRef windowListAll = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    NSArray* arr = CFBridgingRelease(windowListAll);
    // Loop through the windows
    for (NSMutableDictionary* entry in arr){
        if (entry == nil){
            break;
        }
        //NSLog(@"enter:%@",entry);
        NSString *ownerName=[entry objectForKey:(id)kCGWindowOwnerName];
        std::string wndOwnerName([ownerName UTF8String]);
        NSInteger wndNumber=[[entry objectForKey:(id)kCGWindowNumber] intValue];
        //NSLog(@"wndName:%@ number:%ld",wndName,wndNumber);
        //下面这个方法是手册中最先查到的，但仅对属于自己app的窗口有效，其它app的窗口无效，所以不能采用
        NSWindow * wind=[NSApp windowWithWindowNumber: wndNumber];
//        NSLog(@"wnd:%@",wind);
//        NSLog(@"wnd id:%@",wind.windowNumber);
        if (wndNumber == wind.windowNumber){
            //qDebug()<<"Skip cureent window number,it is "<<wind.windowNumber;
            // 去除当前应用窗口
            continue;
        }

        CGRect bounds;
        QRect windowRcet;
        CGRectMakeWithDictionaryRepresentation((CFDictionaryRef)[entry objectForKey:@"kCGWindowBounds"], &bounds);
        //NSLog(@"bounds: %@",NSStringFromRect(bounds));
        windowRcet.setX(bounds.origin.x);
        windowRcet.setY(bounds.origin.y);
        windowRcet.setWidth(bounds.size.width);
        windowRcet.setHeight(bounds.size.height);

//        //根据pid获取窗口所属的app
//        pid_t pid = [[entry objectForKey:(id)kCGWindowOwnerPID] intValue];
//        AXUIElementRef appRef = AXUIElementCreateApplication(pid);
//        //NSLog(@"Ref = %@",appRef);
//        //获取app所有的窗口
//        CFArrayRef windowList;
//        AXUIElementCopyAttributeValue(appRef, kAXWindowsAttribute, (CFTypeRef *)&windowList);
//        //NSLog(@"WindowList = %@", windowList);
//        CFRelease(appRef);
//        if (!windowList){
//            //NSLog(@"windowList is nil");
//            continue;
//        }

        auto windowLayer = [[entry objectForKey:(id)kCGWindowLayer] intValue];

        // 判断当前鼠标指针是否在该窗口区域内
        if (windowRcet.contains(curPos) && 0 == windowLayer){
//            qDebug()<<"Find window rect"<<windowRcet;
//            qDebug()<<"Window layer is "<<windowLayer;
//            qDebug()<<"application name is "<<wndOwnerName.c_str();
//            qDebug()<<"Find rect is "<< windowRcet;
            appName=wndOwnerName.c_str();
//            qDebug()<<"app name is "<<appName;
            return windowRcet;
        }
//        for (int i=0;i<CFArrayGetCount(windowList);i++){
//            //遍历app所有窗口，查找跟全局遍历所获得窗口的实体
//            AXUIElementRef windowRef = (AXUIElementRef) CFArrayGetValueAtIndex( windowList, i);
//            NSLog(@"windowRef:%@",windowRef);
//            CGWindowID application_window_id = 0;
//            //_AXUIElementGetWindow(windowRef, &application_window_id);
////            if (application_window_id == wndNumber){
////                //找到
////                NSLog(@"Found a wnd that number is:%u",application_window_id);
////                //根据需要来操作窗口的位置，仅用作示例，这里可以修改成其它操作
////                CFTypeRef position;
////                CGPoint newPoint;
////                newPoint.x = 0;
////                newPoint.y = 0;
////                NSLog(@"Create new position");
////                position = (CFTypeRef)(AXValueCreate(kAXValueCGPointType, (const void *)&newPoint));
////                //setting new position
////                AXUIElementSetAttributeValue(windowRef, kAXPositionAttribute, position);
////            }
//            CFRelease(windowRef);
//        }
//     NSLog(@"end a loop ----------------------------");
    } //for windowListAll
    } //autorelease

    auto screen = QGuiApplication::screenAt(curPos);
    if(screen){
        return screen->geometry();
    }

    return QGuiApplication::primaryScreen()->geometry();
}
```

## 获取硬件设备变更的消息

- [通过IOKit监听Mac电脑上的USB驱动事件_AA岁月静好AA-CSDN博客](https://blog.csdn.net/zzl819954692/article/details/89704373)
- [Monitor USB drive events on Mac through IOKit](https://laptrinhx.com/monitor-usb-drive-events-on-mac-through-iokit-257426858/)

# 关机，睡眠，自启动

```objectivec
#include "systemctlmac.h"
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>

// Link1: https://stackoverflow.com/questions/4505632/shutdown-mac-objective-c
// Link2: https://blog.csdn.net/sodaslay/article/details/7869709

SystemCtlMac::SystemCtlMac()
{

}

ERE_CODE SystemCtlMac::shutdown()
{
    NSString * action = @"shut down";
    return exeScriptAction(QString::fromNSString(action));
}

ERE_CODE SystemCtlMac::sleep()
{
    NSString * action = @"sleep";
    return exeScriptAction(QString::fromNSString(action));
}

bool SystemCtlMac::isLaunchAtStartup()
{
    bool ret=false;
    NSString* launchFolder = [NSString stringWithFormat:@"%@/Library/LaunchAgents",NSHomeDirectory()];
    NSString * boundleID = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleIdentifierKey];
    NSString* dstLaunchPath = [launchFolder stringByAppendingFormat:@"/%@.plist",boundleID];
    NSFileManager* fm = [NSFileManager defaultManager];
    BOOL isDir = NO;
    if ([fm fileExistsAtPath:dstLaunchPath isDirectory:&isDir] && !isDir) {
        ret = true;
    }
    return ret;
}

void SystemCtlMac::setLaunchAtStartup(bool launch, const std::wstring &applicationPath)
{
    Q_UNUSED(applicationPath);
    if(launch){
        NSString* launchFolder = [NSString stringWithFormat:@"%@/Library/LaunchAgents",NSHomeDirectory()];
        NSString * boundleID = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleIdentifierKey];
        NSString* dstLaunchPath = [launchFolder stringByAppendingFormat:@"/%@.plist",boundleID];
        NSFileManager* fm = [NSFileManager defaultManager];
        BOOL isDir = NO;
        //已经存在启动项中，就不必再创建
        if ([fm fileExistsAtPath:dstLaunchPath isDirectory:&isDir] && !isDir) {
            return;
        }
        //下面是一些配置
        NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
        NSMutableArray* arr = [[NSMutableArray alloc] init];
        [arr addObject:[[NSBundle mainBundle] executablePath]];
        [arr addObject:@"-runMode"];
        [arr addObject:@"autoLaunched"];
        [dict setObject:[NSNumber numberWithBool:true] forKey:@"RunAtLoad"];
        [dict setObject:boundleID forKey:@"Label"];
        [dict setObject:arr forKey:@"ProgramArguments"];
        isDir = NO;
        if (![fm fileExistsAtPath:launchFolder isDirectory:&isDir] && isDir) {
            [fm createDirectoryAtPath:launchFolder withIntermediateDirectories:NO attributes:nil error:nil];
        }
        [dict writeToFile:dstLaunchPath atomically:NO];
        [arr release];  arr = nil;
        [dict release]; dict = nil;
    }else{
        //取消开机启动则只要删除~/Library/LaunchAgents/ 目录下相应的plist文件即可
        NSString* launchFolder = [NSString stringWithFormat:@"%@/Library/LaunchAgents",NSHomeDirectory()];
        BOOL isDir = NO;
        NSFileManager* fm = [NSFileManager defaultManager];
        if (![fm fileExistsAtPath:launchFolder isDirectory:&isDir] && isDir) {
            return;
        }
        NSString * boundleID = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleIdentifierKey];
        NSString* srcLaunchPath = [launchFolder stringByAppendingFormat:@"/%@.plist",boundleID];
        [fm removeItemAtPath:srcLaunchPath error:nil];
    }

}

ERE_CODE SystemCtlMac::exeScriptAction(const QString& action)
{
    ERE_CODE ereRet = ERE_CODE_FAIL;
    NSString *scriptSource = [NSString stringWithFormat:@"tell application \"Finder\" to %@", action.toNSString()];
    NSAppleScript* theScript = [[NSAppleScript alloc] initWithSource:scriptSource];
    if (theScript != NULL)
    {
        NSDictionary* errDict = NULL;
        // execution of the following line ends with EXC
        if (YES == [theScript compileAndReturnError: &errDict])
        {
            NSLog(@"compiled the script");
            [theScript executeAndReturnError: &errDict];
            ereRet = ERE_CODE_SUCCESS;
        }
        [theScript release];
    }
    return ereRet;
}
```

# 其它问题

在多个屏幕的环境下，识别区域放大镜从一个屏幕快速移动到另外一个屏幕后，有大概率的情况会在另一个屏幕上留下一个透明的放大镜烙印（且在 mousemoveevent 中调用 update（）会降低该概率），比如这样

解决方法：设置 Qt::NoDropShadowWindowHint 属性

# 使用 native app Delegate

[macOS: how to get access to the NSApplicationDelegate?](https://forum.qt.io/topic/102089/macos-how-to-get-access-to-the-nsapplicationdelegate)

[Apple Developer Documentation](https://developer.apple.com/documentation/appkit/nsapplicationdelegate?language=objc)

[macOS - AppDelegate](https://www.jianshu.com/p/7a91d2e04ef5)

[macOS / Qt: Disabling the "Quit" menu item on my application's dock menu](https://stackoverflow.com/questions/57048259/macos-qt-disabling-the-quit-menu-item-on-my-applications-dock-menu)

## 示例代码

```objectivec
#ifndef APPDELEGATE_MAC_H
#define APPDELEGATE_MAC_H
#include <QObject>

class AppDelegate_mac:public QObject
{
    Q_OBJECT
public:
    AppDelegate_mac();
    void installDelegate();
signals:
    void appQuitSig();
    void clickedOnDock();
};

#endif // APPDELEGATE_MAC_H
```

```objectivec
#include "appdelegate_mac.h"
#import <Appkit/NSApplication.h>
#import <Appkit/Appkit.h>

//链接：https://www.jianshu.com/p/7a91d2e04ef5

@interface AppDelegate:NSObject<NSApplicationDelegate>{
   AppDelegate_mac * m_notifer;
}
+ (AppDelegate *)getInstance;
- (void)setNotifer:(AppDelegate_mac * )notifer;
@end

AppDelegate *instance = nil;

@implementation AppDelegate

+(AppDelegate *)getInstance
{
    if (instance == nil) {
        instance = [[AppDelegate alloc]init];
    }
    return instance;
}

- (void)setNotifer:(AppDelegate_mac * )notifer{
    m_notifer=notifer;
}

#pragma mark 程序坞（dock）上的图标被点击了
- (BOOL)applicationShouldHandleReopen:(NSApplication *)sender hasVisibleWindows:(BOOL)flag
{
    if (flag) {
        return YES;
    }
    emit m_notifer->clickedOnDock();
    return YES;
}

#pragma mark 应用程序将要退出，一般在提示重要数据未保存时使用
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    NSLog(@"Application should terminate...");
    emit m_notifer->appQuitSig();
    return NSTerminateCancel;
}

#pragma mark 应用程序将要退出，清理缓存和关闭某些功能时用到
- (void)applicationWillTerminate:(NSNotification *)aNotification {
    NSLog(@"Application will terminate...");
}
@end

AppDelegate_mac::AppDelegate_mac()
{

}

void AppDelegate_mac::installDelegate()
{
    AppDelegate * instance = [ AppDelegate getInstance ];
    [instance setNotifer:this ];
    [NSApp setDelegate:instance];
}
```

警告：同 QSystemTray 有冲突，oops，具体表现为主窗口未激活时托盘菜单是被禁用的，oops

## 动态获取用户是否允许相关权限：

[macOS 开发 - 10.15 Screen Recording 屏幕录制 授权及跳转_伊织_的博客-CSDN博客](https://blog.csdn.net/lovechris00/article/details/96979960)

[In Catalina running bash script vi... | Apple Developer Forums](https://developer.apple.com/forums/thread/123489)

[In Catalina running bash script vi... | Apple Developer Forums](https://developer.apple.com/forums/thread/123489)

[macOS: List apps authorized for full disk access](https://apple.stackexchange.com/questions/362865/macos-list-apps-authorized-for-full-disk-access/372596#372596)

允许屏幕录制后，不需要重启，就可以使用（QQ，嗨格式）都可以，其他的不行

[Mac开发-10.14完全磁盘访问权限获取PermissionKit_CaicaiNo.1-程序员宅基地 - 程序员宅基地](https://cxyzjd.com/article/shengpeng3344/103498486)

[macOS 10.15截图权限检测](https://confiwang.github.io/2020/01/08/macOS%2010-15%E6%88%AA%E5%9B%BE%E6%9D%83%E9%99%90%E6%A3%80%E6%B5%8B/)

[iOS check if application has access to microphone](https://stackoverflow.com/questions/24981333/ios-check-if-application-has-access-to-microphone)

[Unable to see apps in Microphone under Privacy Settings on MacBook Air Catalina 10.15.4](https://answers.microsoft.com/en-us/msoffice/forum/all/unable-to-see-apps-in-microphone-under-privacy/531e730a-c607-4e2e-ae59-10161481b3a5)


[Qt for macOS - Deployment](https://doc.qt.io/qt-5/macos-deployment.html)

## MAC 打包 PKG

[macOS 开发 - 打包 PKG（Packages）_伊织的小花园-CSDN博客_macos 打包pkg](https://blog.csdn.net/lovechris00/article/details/84546751)

# 在 mac 上 编译 QT 5.15.2，让其能够生成 universal 程序

主要思路如下：

1. 编译 arm 版本的 qt
2. 编译 x86 版本的qt
3. 使用第三方工具合并 x86 和 arm 为 一
4. 安装 universal  版本 qt creator

## 编译生成qt版本（需要在 arm 设备和 x86 设备上分别编译）：

1. 安装必备的工具 
    
    ```bash
    brew install pcre2 harfbuzz freetype
    brew install cmake ninja python llvm   
    ```
    
2. 下载qt 源码，比如 

[](https://download.qt.io/archive/qt/5.15/5.15.2/single/qt-everywhere-src-5.15.2.tar.xz)

1. 解压文件，创建目录

```bash
#解压此压缩包，在压缩包解压文件夹同一层目录创建构建目录：          
mkdir  buildir
#然后在同一层目录下创建编译目标路径          
mkdir outDir
#进入构建目录        
cd  buildir
```

1. 编译配置 

```bash
#for m1 mac 
../qt-everywhere-src-5.15.2/configure -release -prefix ./outDir QMAKE_APPLE_DEVICE_ARCHS=arm64 -opensource -confirm-license -skip qt3d -skip qtwebengine
#for x86 mac
../qt-everywhere-src-5.15.2/configure -release -prefix ./outDir QMAKE_APPLE_DEVICE_ARCHS=x86_64 -opensource -confirm-license -skip qt3d -skip qtwebengine

```

1. 编译，然后安装 

```bash
make -j15
make install
```

## 合并两个版本，制作 universal 版本 Qt

做成x86版本的Qt后需要将两个Qt版本合并为universal版Qt,参考  

[Apple Developer Documentation](https://developer.apple.com/documentation/apple-silicon/building-a-universal-macos-binary)

用 lipo 工具可以将arm与x86程序融合成universal版本程序,Qt的工具集很多，一个一个操作相当费时.

GitHub上有个工具相当方便 

[GitHub - nedrysoft/makeuniversal: Tool to create a Universal Binary version of a Qt distribution.](https://github.com/nedrysoft/makeuniversal)

用makeuniversal工具可以合并两个文件夹为universal.

## 编译 universal 程序

可在工程配置文件 ***.pro 添加：

```bash
mac:{
	QMAKE_APPLE_DEVICE_ARCHS=x86_64 arm64
}
```

，此时生成的执行文件。用 file 命令查看就会显示诸如：

```bash
untitled10: Mach-O universal binary with 2 architectures: [x86_64:Mach-O 64-bit executable x86_64] [arm64:Mach-O 64-bit executable arm64]
untitled10 (for architecture x86_64):	Mach-O 64-bit executable x86_64
untitled10 (for architecture arm64):	Mach-O 64-bit executable arm64
```

# 签名编译好的 qt 相关库，要不在其他电脑上使用该版本会有提示无法验证的问题

## 安装 universal  版本 qt creator

```bash
brew install qt-creator-dev
```

## Qt creator 中添加新的 kits

添加 新的 Qt Version - Qt 5.15.2(universal);

添加新的 kit；更改 编译器 为 Apple Clang(Arm64) 版本；选中 Qt 5.15.2(universal);然后保存即可；


- [wqc_wood](https://www.cnblogs.com/wqcwood/p/15138983.html)
- [MAC M1 QT-kits配置 QT安装 qt-version配置 No suitable kits found. The qmake executable could not be added_影子墨的博客-CSDN博客_qt kits](https://blog.csdn.net/weixin_44165203/article/details/124963805)

## 使用 install_name_tool 后，一些库的签名失效。运行 程序加载动态库崩溃

解决方案： 

```cpp
codesign --force -s - /path/to/dylib
```

[install_name_tool errors on arm64](https://stackoverflow.com/questions/71744856/install-name-tool-errors-on-arm64)


## 查看签名相关
在研究如何在目标应用中控制代码执行流前，我们需要使用`codesign`来验证与该程序关联的一些标志：

`codesign -d --entitlements :- /System/Library/PrivateFrameworks/IMCore.framework/imagent.app -vv`

执行该命令后，我们可以看到如下元数据：

`CodeDirectory v=20100 size=4066 flags=0x0(none) hashes=120+5 location=embedded`

在寻找合适的目标应用时，我们要注意一些标志，其中包括`library-validation`，这个标志表示只有经过Apple或者应用team ID签名的dylib才能被加载。此外还有`runtime`标志，表示应用使用的是比较安全的运行时，同样不允许我们将自己的dylib加载到进程中。

## 相关链接

- [Compile boost as universal library (Intel and Apple Silicon architectures)](https://stackoverflow.com/questions/64553398/compile-boost-as-universal-library-intel-and-apple-silicon-architectures)
- [Qt Creator for Mac：升级Xcode导致工程编译出错_Will. Liu的博客-CSDN博客](https://blog.csdn.net/lgyxdn/article/details/105999096)
- [How to get a list of all open NSWindow from all running application?](https://stackoverflow.com/questions/4036420/how-to-get-a-list-of-all-open-nswindow-from-all-running-application)
- [Get all windows in Qt](https://stackoverflow.com/questions/20729965/get-all-windows-in-qt)
- [How to get running window informations in macOS · Issue #243 · onmyway133/blog](https://hub.fastgit.org/onmyway133/blog/issues/243)
- [两种方法操作其它mac应用的窗口](https://www.cnblogs.com/andrewwang/p/8635292.html)
- [OS X Frameworks](https://developer.apple.com/library/archive/documentation/MacOSX/Conceptual/OSX_Technology_Overview/SystemFrameworks/SystemFrameworks.html)
- [如何绕过macOS隐私控制机制](https://www.anquanke.com/post/id/187307)

---

- [上一级](README.md)
- 上一篇 -> [mac 开发资料](macDevelopSome.md)
