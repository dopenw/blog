<!-- 使用 visual studio 遇到的一些问题 -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [error Please #define _AFXDLL or do not use /MD[d]](#error-please-define-_afxdll-or-do-not-use-mdd)
- [VS调试 不会命中断点，源代码与原始版本不同](#vs调试-不会命中断点源代码与原始版本不同)

<!-- /code_chunk_output -->


## error Please #define _AFXDLL or do not use /MD[d]

[error Please #define _AFXDLL or do not use /MD[d] occurs even after making changes in Project Properties](https://stackoverflow.com/questions/25031009/error-please-define-afxdll-or-do-not-use-mdd-occurs-even-after-making-chang)

## VS调试 不会命中断点，源代码与原始版本不同
解决办法：
- 清理解决方案 -> 重新生成 -> 打开受影响文档 -> Ctrl + A -> (Ctrl + K,Ctrl + F) -> 重新生成。（注：可能会造成大量的格式变更）
- 将受影响的代码文件转存为 UTF-8 with BOM 格式。

参考连接：[VS调试 不会命中断点，源代码与原始版本不同的解决办法](http://www.predream.org/show-126-848-1.html)


---
- [上一级](README.md)
- 上一篇 -> [Win 10 插入U盘后，听见插入提示音，但在我的电脑里没有相应的入口](insertUSBDevicesNotResponse.md)
- 下一篇 -> [visual studio 创建和使用 DLL 文件](visualStudioCreateUseDLL.md)
