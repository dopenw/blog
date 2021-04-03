# 使用 MFCMigrationFramework


## MFCMigrationFramework 简介

MFCMigrationFramework 是 Qt 提供的一个用于让 MFC 和 Qt 程序能够在同一个应用中工作的框架。

github:[qtproject/qt-solutions/qtwinmigrate](https://github.com/qtproject/qt-solutions/tree/master/qtwinmigrate)


### 创建一个可供 MFC 程序使用的动态库（该动态库利用Qt实现）

注意： 截止 github 上该框架的最后一次提交(qtwinmigrate: Fix geometry offset) 16 Jul 2018  ,该版本在 Qt5.11 上工作得不太好，其他Qt5 版本没有测试，但我改为使用 Qt4.8.6 一切正常。

1. 创建一个 Qt 动态库工程
2. 使用 MFCMigrationFramework 动态库（MFCMigrationFramework 提供了一个 buildlib 工程）
3. 使用如下代码：

```c++
// 使用 MFC_DLL 来表示生成可供 MFC 应用程序使用的动态库
// ＃else 即表示生成供 Qt应用程序 使用的动态库
#ifdef MFC_DLL
#include <Windows.h>
#include <qwinwidget.h>
#pragma comment  (lib,"User32.lib")
#endif

#ifdef MFC_DLL
    class   SpectrumWidget : public QWinWidget,public SpectrumInterface
#else
    class   SpectrumWidget : public QWidget,public SpectrumInterface
#endif
{
  #ifdef MFC_DLL
        SpectrumWidget(HWND hParentWnd, QObject *parent = 0, Qt::WindowFlags f = 0 );
  #else
        SpectrumWidget( QWidget *parent = nullptr );
  #endif
}

#ifdef MFC_DLL
SpectrumWidget::SpectrumWidget(HWND hParentWnd, QObject * parent , Qt::WindowFlags f ):
     QWinWidget(hParentWnd,parent,f)
#else
SpectrumWidget::SpectrumWidget( QWidget * parent ):
    QWidget( parent )
#endif
{
  ...
}

// MFC 版本需要这个函数
void SpectrumWidget::moveSize(long x, long y, int width, int height)
   {
#ifdef MFC_DLL
       resize(width,height);
       setFixedSize(width,height);
       move(x,y);
#endif
   }

// 这里给出 SpectrumInterface 的相关实现
SpectrumWidget * pSpectrumWidget = nullptr;
//QApplication * g_app = nullptr;
SpectrumInterface*	SpectrumInterface::createSpectrum(long parentHandle)
{
#ifdef MFC_DLL
    QMfcApp::pluginInstance();
    //int argc = 0;
    //g_app = new QApplication(argc,nullptr);
    pSpectrumWidget = new SpectrumWidget(reinterpret_cast<HWND>(parentHandle));
    pSpectrumWidget->show();
    return pSpectrumWidget;
#else
    return new SpectrumWidget(reinterpret_cast<QWidget*>(parentHandle));
#endif
}

void SpectrumInterface::closeSpectrum()
{
#ifdef MFC_DLL
    delete  pSpectrumWidget;
    delete qApp;
#endif
}
```
---
- [上一级](README.md)
- 上一篇 -> [编译 vs2012 qt 5.3.1 64 位版本](useCompiledQt.md)
