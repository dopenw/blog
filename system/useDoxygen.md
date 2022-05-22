<!-- 使用 doxygen   -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [doxygen 简介](#doxygen-简介)
- [下载 doxygen](#下载-doxygen)
- [创建配置文件，并生成文档](#创建配置文件并生成文档)
- [Link](#link)

<!-- /code_chunk_output -->

## doxygen 简介

[Doxygen](https://www.doxygen.nl/index.html) is a documentation generator and static analysis tool for software source trees. When used as a documentation generator, Doxygen extracts information from specially-formatted comments within the code. When used for analysis, Doxygen uses its parse tree to generate diagrams and charts of the code structure. Doxygen can cross reference documentation and code, so that the reader of a document can easily refer to the actual code.


## 下载 doxygen 

For fedora:
```sh
dnf install doxygen
```

在 windows 上安装完成后，若想使用类图等功能，还需安装 [Graphviz - Graph Visualization Software](https://graphviz.org/)


## 创建配置文件，并生成文档

- Creating a configuration file
```sh
doxygen -g $CONFIG_FILE_NAME
```
- Modify some items in the configuration file to the following values：
```ini
EXTRACT_ALL            = YES 
INPUT                  = ./stacer ./stacer-core README.md
RECURSIVE              = YES 
USE_MDFILE_AS_MAINPAGE = README.md
SOURCE_BROWSER         = YES
INLINE_SOURCES         = YES
HAVE_DOT               = YES 
UML_LOOK               = YES
```
- Running doxygen
```sh
doxygen $CONFIG_FILE_NAME
```



## Link 

- [Doxygen wikipedia](https://en.wikipedia.org/wiki/Doxygen)
- [doxygen - Getting started](https://www.doxygen.nl/manual/starting.html)
- [Use the README.md file as main page in Doxygen](https://stackoverflow.com/questions/13368350/use-the-readme-md-file-as-main-page-in-doxygen)
- [How to use doxygen to create UML class diagrams from C++ source](https://stackoverflow.com/questions/4755913/how-to-use-doxygen-to-create-uml-class-diagrams-from-c-source)
- [doxygen - Output Formats](https://www.doxygen.nl/manual/output.html)
- [error using doxygen and graphViz: problems opening map file](https://stackoverflow.com/questions/24158788/error-using-doxygen-and-graphviz-problems-opening-map-file)

---
- [上一级](README.md)
- 上一篇 -> [数据结构（树）](tree.md)
- 下一篇 -> [用户模式和内核模式](userModeAndKernelMode.md)
