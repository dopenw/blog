<!-- Qt Creator 使用 -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [使用 code format -> ClangFormat](#使用-code-format-clangformat)
- [Link](#link)

<!-- /code_chunk_output -->

## 使用 code format -> ClangFormat
1. (For Windows) 安装 [LLVM](https://llvm.org/builds/) (其中包含 Clang-format ,也可直接安装 Clang-format),安装时要勾选添加到系统路径 
2. 进入 Qt Creator Help -> About plugins,并勾选 Beautifier.
3. 进入 Tools -> Options -> Environment -> Keyboard，找到 ClangFormat -> formatFile ，并设置快捷键，我这里设置为 `Ctrl+Alt+F`
4. 重启 Qt Creator 
5. 若重启后出现 `lldb-vscode.exe/lldb.exe `系统错误，找不到 python36.dll,无法继续执行代码。重新安装程序会解决此问题。下载 [dowload a python 3.6 prebuilt package](https://www.python.org/ftp/python/3.6.4/python-3.6.4-embed-amd64.zip) ,将 其中的 python36.dll、python3.dll 放到 `$LLVM_dir/bin` 目录下即可解决该问题


## Link 
* [What is the shortcut to format code in Qt Creator?](https://stackoverflow.com/questions/37597117/what-is-the-shortcut-to-format-code-in-qt-creator)
* [dll error when starting Qt Creator 4.8.2](https://forum.qt.io/topic/109164/dll-error-when-starting-qt-creator-4-8-2)

---
- [上一级](README.md)
- 上一篇 -> [23. 平台相关性](23_platformDependencies.md)
- 下一篇 -> [《C++ Qt 设计模式（第二版）》 读书笔记](cppQtDesignPatternsBook.md)
