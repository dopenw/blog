# Visual studio 安装 Visual Assist


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [Visual studio 安装 Visual Assist](#visual-studio-安装-visual-assist)
	- [Visual Assist 概览](#visual-assist-概览)
	- [crack visual Assist](#crack-visual-assist)
	- [VisualStudio 2008 install visual assist](#visualstudio-2008-install-visual-assist)
	- [使用 Trial-Reset40 重置试用期](#使用-trial-reset40-重置试用期)
	- [Link](#link)

<!-- /code_chunk_output -->


## Visual Assist 概览

Visual Assist具有丰富的功能来提高开发人员的生产力。借助Visual Assist包含的众多功能，可以让您的 visual stduio 更好用。

[Visual Assist offical website](https://www.wholetomato.com/default.asp)

## crack visual Assist

[Download crack Files](https://www.worldsrc.com/programs/crack.php?Ty=429&n=crack_For_Whole_Tomato_Visual_Assist_X)

Open crack file,and you can see this:

```highlight
INSTALLATION

0) Uninstall VA (if you have installed it before).
0.0) Manually check for alive _folders_ of pervious version of VA in extensions folders (paths are described
below) and remove them by yourself, because of VA uninstaller's bug
1) Locate all places where va_x.dll's are reside in your MSVCs (paths are described below)
2) Replace all found va_x.dll with our one
3) PROFIT (you may see "License: trial" - don't worry, all should work without any limitation)


Places where Visual Assist extension (va_x.dll and another stuff) resides in different versions of MS Visual Studio are listed below:

-------------------
MSVC version | path
-------------------
msvc2008     | find in installation path (by default c:\Program Files (x86)\Visual Assist X\)

msvc2010     | %USERPROFILE%\AppData\Local\Microsoft\VisualStudio\10.0\Extensions\Whole Tomato Software\Visual Assist\__version__\

msvc201[1|2] | %USERPROFILE%\AppData\Local\Microsoft\VisualStudio\11.0\Extensions\__random_dir__\

msvc2013     | %USERPROFILE%\AppData\Local\Microsoft\VisualStudio\12.0\Extensions\__second_random_dir__\

msvc2015     | %USERPROFILE%\AppData\Local\Microsoft\VisualStudio\14.0\Extensions\__random_dir__\

msvc2017     | %USERPROFILE%\AppData\Local\Microsoft\VisualStudio\15.0__%XXX%\Extensions\__second_random_dir__\
-------------------

Notes: in my case
__version__ is "10.9.2238.2"
__random_dir__ and __second_random_dir__ are generated and looks like "v3tpxirz.5pr"

WorldSrc.com
```

## VisualStudio 2008 install visual assist

以管理员权限运行该安装程序，运行后如下图所示：

![](../images/VisualStudioInstallVisualAssist_201805312037_1.PNG)

点击安装就可以给 VisualStudio 2008 安装 Visual assist。

## 使用 Trial-Reset40 重置试用期
1. [下载Trial-Reset40 ](https://www.raymond.cc/blog/download/did/278/)
2. 以管理员权限运行 `Trial-Reset.exe`
3. 点击左侧工具栏的 Armadillo 按钮，等待扫描结束，选中右侧列表所有 key，右键选择 `Deleter key`

注意：若提示 `Component ‘MSCOMCTL.OCX’ or one of its dependencies not correctly registered: a file is missing or invalid`:
1. [Download MSCOMCTL.OCX](https://www.ocxme.com/files/mscomctl_ocx)
2. for 64 bits:拷贝 MSCOMCTL.OCX 到 `C:\Windows\SysWOW64`
3. for 32 bits:拷贝 MSCOMCTL.OCX 到 `C:\Windows\System32`
4. 以管理员模式启动 cmd 程序
5. 执行 `regsvr32 mscomctl.ocx`


## Link 
* [5 Ways to Extend Trial Periods of Shareware Software](https://www.raymond.cc/blog/how-to-extend-the-trial-period-of-a-software/)
* [Fix: Component ‘MSCOMCTL.OCX’ or one of its dependencies not correctly registered: a file is missing or invalid](https://appuals.com/fix-component-mscomctl-ocx-one-dependencies-not-correctly-registered-file-missing-invalid/)

---
- [上一级](README.md)
- 上一篇 -> [将中文docx文档转换为英文文档](DocZhToEn.md)
- 下一篇 -> [Win 10 插入U盘后，听见插入提示音，但在我的电脑里没有相应的入口](insertUSBDevicesNotResponse.md)
