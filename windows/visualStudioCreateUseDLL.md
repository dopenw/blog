# visual studio 创建和使用 DLL 文件


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [visual studio 创建和使用 DLL 文件](#visual-studio-创建和使用-dll-文件)
	* [创建 DLL 文件](#创建-dll-文件)
	* [将执行文件链接到到 DLL](#将执行文件链接到到-dll)
		* [隐式链接](#隐式链接)
		* [显式链接](#显式链接)
	* [利用 Dependency Walker 查看依赖关系](#利用-dependency-walker-查看依赖关系)
	* [将 vs2008 DLL project update vs2010 DLL project](#将-vs2008-dll-project-update-vs2010-dll-project)
	* [Process Monitor](#process-monitor)
	* [参考链接](#参考链接)

<!-- /code_chunk_output -->


## 创建 DLL 文件

[Creating and Using a Dynamic Link Library (C++)](https://msdn.microsoft.com/en-us/library/ms235636.aspx)

## 将执行文件链接到到 DLL

[Linking an Executable to a DLL](https://msdn.microsoft.com/en-us/library/9yd93633.aspx)

可执行文件通过以下两种方式之一链接到（或加载）DLL:
* [隐式链接](https://msdn.microsoft.com/en-us/library/d14wsce5.aspx)
* [显式链接](https://msdn.microsoft.com/en-us/library/784bt7z7.aspx)

隐式链接有时被称为静态加载或加载时动态链接。显式链接有时被称为动态加载或运行时动态链接

### 隐式链接
将上面生成的 DLL 文件对应的 .lib 文件包含到需要使用 DLL 文件的 project 中，并把 .dll 文件和可执行文件放到一起即可。
[Youtube -- Implicit loading of Dll file using Visual Studio](https://www.youtube.com/watch?v=rS_lE2UOzgg)

### 显式链接

示例：
```c++
typedef UINT (CALLBACK* LPFNDLLFUNC1)(DWORD,UINT);  
...  

HINSTANCE hDLL;               // Handle to DLL  
LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer  
DWORD dwParam1;  
UINT  uParam2, uReturnVal;  

hDLL = LoadLibrary("MyDLL");  
if (hDLL != NULL)  
{  
   lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,  
                                           "DLLFunc1");  
   if (!lpfnDllFunc1)  
   {  
      // handle the error  
      FreeLibrary(hDLL);         
      return SOME_ERROR_CODE;  
   }  
   else  
   {  
      // call the function  
      uReturnVal = lpfnDllFunc1(dwParam1, uParam2);  
   }  
}  
```

## 利用 Dependency Walker 查看依赖关系

Dependency Walker是一个免费的实用程序，可扫描任何32位或64位Windows模块（exe，dll，ocx，sys等），并构建所有相关模块的分层树状图。对于找到的每个模块，它列出了该模块导出的所有功能，以及其中哪些功能实际上由其他模块调用。另一个视图显示所需文件的最小集合，以及有关每个文件的详细信息，包括文件的完整路径，基本地址，版本号，机器类型，调试信息等。对于加载和执行模块相关的系统错误，Dependency Walker也非常有用。 Dependency Walker检测许多常见的应用程序问题，如缺少模块，无效模块，导入/导出不匹配，循环依赖性错误，模块的机器类型不匹配以及模块初始化失败。 Dependency Walker可在Windows 95,98，Me，NT，2000，XP，2003，Vista，7和8上运行。它可以处理任何32位或64位Windows模块，包括专为Windows CE设计的模块。

[Dependency Walker official](http://www.dependencywalker.com/)


## 将 vs2008 DLL project update vs2010 DLL project
将 vs 2008 dll 升级成 vs 2010 dll，使用 v100 编译后，将该 dll 放置到其他 Project 里面，调用该 dll 出错，错误码 127；

后经过多次尝试，使用 v100 编译后的dll 错误依旧；最后无奈只能将 vs2010 project 编译器改为 v90（仅升级 Project 到 2010）,能够成功使用；

下面列出从 stack overflow 上找到的解决 dll 错误的方法：

1. The error message means that the dll was found but a required function is missing. (Jitter is right.) This implies that you have the dll you need, but not the right version. (Davefiddes is right, although the problem can be any dll, not just the Microsoft runtime library. And, at least for major updates, Microsoft gives its runtime libraries different names, so in that case it wouldn't be an issue.)

2. This doesn't make sense, because no function has been requested from the dll being loaded. (Adam is right.)

3. Therefore, the missing function was expected to be found not in the dll which is being explicitly loaded by the LoadLibrary command, but in a dependent dll which is being implicitly loaded at the same time, because the first dll requires it. (Zebrabox was close.)

4. A dependent dll is a dll that is "statically" linked to the library being explicitly loaded, via an import library, or .lib file, included on the linker step of the explicitly loaded dll. (I bet you didn't know that a "dynamic link library" could be "statically linked." Well, now you do.)

5. If you have multiple versions of the same dll in different folders, then this could also be a search path problem (as zebrabox suggests). Dll path search order is a complicated subject in itself: see http://msdn.microsoft.com/en-us/library/ms682586(VS.85).aspx . It depends on operating system, among other things. The safest bet, where practical, is to put all the potential problem dlls in the same folder as your exe.

6. Dependent dlls can also have their own dependent dlls, which can make this problem very difficult to resolve. Depends might help, but if it doesn't, try filemon. The last dll that's successfully read before your error message is the one that's the wrong version.

后发现该项目生成目录下依赖的qt dll 文件版本不正确，将使用 vs1010 编译好的 qt dll 替换原始 qt dll 文件，问题得到解决。

## Process Monitor

Process Monitor 是windows下高级实时监听工具，用于监视文件系统、注册表、进程和线程的活动。它兼并了两个Sysinternals实用工具Filemon和Regmon的特点，并且增加了一系列的扩展包括丰富而无干扰的过滤全面的事件属性比如会话ID和用户名，可靠的进程信息，全部的线程栈和对每一个操作完整的符号支持，同时一个或多个文件。这些优秀的功能将使Process Monitor成为解决操作系统问题和恶意软件跟踪的重要工具之一。

[【茶包射手專欄】Process Monitor基本操作教學](https://blog.darkthread.net/blog/977/)
[Process Monitor中文手册](https://blog.csdn.net/whatday/article/details/8758380)

注：可以使用该工具检查 dll 调用情况；

## 参考链接
* [为何Windows下的动态库总伴随一个静态库？](http://blog.shengbin.me/posts/windows-dll-with-lib)
* [LoadLibrary() error code 127](https://stackoverflow.com/questions/1053180/loadlibrary-error-code-127)
* [LoadLibrary() fails with Error code 127](https://social.msdn.microsoft.com/Forums/vstudio/en-US/e3911d17-17b9-4911-969d-670ce5db6e45/loadlibrary-fails-with-error-code-127?forum=vcgeneral)
* [从vb6加载DLL时找不到文件](https://codeday.me/bug/20181004/278067.html)

[上一级](README.md)
[上一篇](useVisualStudio.md)
[下一篇](vmvare_windows.md)
