# visual studio 创建和使用 DLL 文件


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [visual studio 创建和使用 DLL 文件](#visual-studio-创建和使用-dll-文件)
	* [创建 DLL 文件](#创建-dll-文件)
	* [将执行文件链接到到 DLL](#将执行文件链接到到-dll)
		* [隐式链接](#隐式链接)
		* [显式链接](#显式链接)
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

## 参考链接
* [为何Windows下的动态库总伴随一个静态库？](http://blog.shengbin.me/posts/windows-dll-with-lib)
