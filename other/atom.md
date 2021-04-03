# Atom


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Atom](#atom)
	* [Atom 简介](#atom-简介)
	* [优秀的插件](#优秀的插件)
		* [atom-beautify](#atom-beautify)
		* [ask-stack](#ask-stack)
		* [atom-clock](#atom-clock)
		* [atom-html-preview](#atom-html-preview)
		* [goto-definition](#goto-definition)
		* [highlight-selected](#highlight-selected)
		* [language-lua](#language-lua)
		* [linter-XXX](#linter-xxx)
		* [markdown-preview-enhanced](#markdown-preview-enhanced)
		* [minimap](#minimap)
		* [minimap-highlight-selected](#minimap-highlight-selected)
		* [minimap-linter](#minimap-linter)
		* [platformio-ide-terminal](#platformio-ide-terminal)
		* [symbols-tree-view](#symbols-tree-view)
		* [file-icons](#file-icons)
		* [docblockr](#docblockr)
		* [highlight-line](#highlight-line)
	* [设置代理](#设置代理)
	* [一键安装](#一键安装)
	* [Link](#link)

<!-- /code_chunk_output -->

## Atom 简介

Atom 编辑器是一个优秀、开源、跨平台的文本编辑器，且具有丰富的插件库和主题库。

![](../images/atom_201801291701_1.png)

[Atom官网](https://atom.io/)

## 优秀的插件

### atom-beautify

[atom-beautify](https://atom.io/packages/atom-beautify)

atom-beautify 是一个代码美化工具，可以让你保存代码时美化代码。

![](../images/atom_201801291701_2.png)

需要安装 clang-format :
* 下载 [LLVM](http://releases.llvm.org/download.html)
* 将上一步中下载好的 Pre-Built Binaries 里面的 bin/clang-format 拷贝到相应的路径下(取决于 atom-beautify 设置里面的执行文件路径)

### ask-stack

The [Ask Stack](https://atom.io/packages/ask-stack) Overflow package allows you to quickly search Stack Overflow and get code samples directly in the Atom editor. To use, simply type ```ctrl + alt + a``` or type “Ask Stack” in the Command Palette.

![](https://i.github-camo.com/8498f81330c46a879f51af467c8b7fba808f87c6/68747470733a2f2f692e696d6775722e636f6d2f796e5a6c4f41542e676966)


### atom-clock
[atom-clock](https://atom.io/packages/atom-clock)

![](../images/atom_201801291701_3.png)

### atom-html-preview

[atom-html-preview](https://atom.io/packages/atom-html-preview)

atom-html-preview 是一个 html 预览工具。

![](../images/atom_201801291701_4.png)

### goto-definition

[goto-definition](https://atom.io/packages/goto-definition)

![](../images/atom_201801291701_5.png)

### highlight-selected

[highlight-selected](https://atom.io/packages/highlight-selected)

Highlights the current word selected when double clicking

![](../images/atom_201801291701_6.png)

### language-lua

[language-lua](https://atom.io/packages/language-lua)

Add syntax highlighting and snippets to Lua files in Atom

### linter-XXX

检查相关语言有没有语法错误

### markdown-preview-enhanced

[markdown-preview-enhanced](https://atom.io/packages/markdown-preview-enhanced)

markdown-preview-enhanced 是一个增强版的 markdown 预览工具

![](../images/atom_201801292325_1.png)

### minimap

[minimap](https://atom.io/packages/minimap)

代码地图

![](../images/atom_201801292325_2.png)

### minimap-highlight-selected

[minimap-highlight-selected](https://atom.io/packages/minimap-highlight-selected)

A minimap binding for the highlight-selected package

![](../images/atom_201801292325_3.png)

### minimap-linter

[minimap-linter](https://atom.io/packages/minimap-linter)

Minimap display for Linter messages

![](../images/atom_201801292325_4.png)

### platformio-ide-terminal

[platformio-ide-terminal](https://atom.io/packages/platformio-ide-terminal)

A terminal package for Atom, complete with themes, API and more for PlatformIO IDE. Fork of terminal-plus.


![](../images/atom_201801292325_5.png)

### symbols-tree-view

[symbols-tree-view](https://atom.io/packages/symbols-tree-view)

A symbols view like taglist

![](../images/atom_201801292325_6.png)

### file-icons

[file-icons](https://atom.io/packages/file-icons)

Assign file extension icons and colours for improved visual grepping

![](../images/atom_201801292325_7.png)

### docblockr

[docblockr](https://atom.io/packages/docblockr)

这个插件可以帮助你快速的生成注释

![](https://i.github-camo.com/385ca4340ba6b85306558d78ecb87e136024985b/68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f4e696b68696c4b616c6967652f646f63626c6f636b722f6d61737465722f7265736f75726365732f6c6f6e672d617267732e676966)

### highlight-line

[highlight-line ](https://atom.io/packages/highlight-line)

Highlights the current line in the editor

## 设置代理

```sh
apm config set https-proxy http://127.0.0.1:8118
```
## 一键安装
```sh
apm config set https-proxy http://127.0.0.1:8118
# 需要先设置代理，要不速度感人
apm install atom-beautify ask-stack atom-clock atom-html-preview goto-definition highlight-selected markdown-preview-enhanced minimap minimap-highlight-selected minimap-linter platformio-ide-terminal symbols-tree-view file-icons docblockr highlight-line
```

## 在 redora 上重装 atom
```sh
$ rm -rf ~/atom
$ rm -rf ~/.atom
$ rm -rf ~/.config/Atom-Shell
$ sudo rm -rf /usr/local/share/atom/
$ dnf autoremove atom
$ rpm -ivh 'atom.x86_64.rpm'
```

## Link

* [开发神器Atom编辑器优秀插件推荐](https://blog.wangjunfeng.com/archives/647)
* [The 20 Best Atom Packages for Front End Developers](https://www.shopify.com/partners/blog/best-atom-packages-2017)
* [Installing Atom](https://flight-manual.atom.io/getting-started/sections/installing-atom/)
* [How to uninstall Atom text editor on Linux?](https://stackoverflow.com/questions/23621181/how-to-uninstall-atom-text-editor-on-linux)


---
- [上一级](README.md)
- 上一篇 -> [利用 Fusion App 创建个人blog安卓客户端](UseFusionAppCreateBlogApp.md)
- 下一篇 -> [安装配置hadoop httpfs](config_hadoop_httpfs.md)
