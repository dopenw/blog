# Git 学习笔记


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [Git 学习笔记](#git-学习笔记)
  - [介绍](#介绍)
  - [安装 Git](#安装-git)
    - [fedora install Git](#fedora-install-git)
    - [windows install Git](#windows-install-git)
  - [起步](#起步)
  - [Git GUI 工具](#git-gui-工具)
    - [Windows](#windows)
    - [Git 命令行](#git-命令行)
    - [Git 使用快速入门](#git-使用快速入门)
      - [创建初始化版本库](#创建初始化版本库)
      - [将文件添加到版本库中](#将文件添加到版本库中)
      - [配置提交作者](#配置提交作者)
      - [再次提交](#再次提交)
      - [查看提交](#查看提交)
      - [查看提交差异](#查看提交差异)
      - [版本库内文件的删除和重命名](#版本库内文件的删除和重命名)
      - [创建版本库副本](#创建版本库副本)
    - [配置文件](#配置文件)
      - [配置别名](#配置别名)
  - [更改提交](#更改提交)
    - [关于修改历史记录的注意事项](#关于修改历史记录的注意事项)
    - [使用 git reset](#使用-git-reset)
      - [revert a commit already pushed to a remote repository](#revert-a-commit-already-pushed-to-a-remote-repository)
  - [git submodule](#git-submodule)
    - [add submodule](#add-submodule)
    - [remove git submodule](#remove-git-submodule)
    - [Git submodule URL not including username?](#git-submodule-url-not-including-username)
    - [遇到的一些问题](#遇到的一些问题)
  - [Link](#link)

<!-- /code_chunk_output -->



该文档是读 “《Git 版本控制管理》 美 Jon Loeliger,Matthew McCullough ” 的学习笔记

## 介绍

Git 的特性：
* 有助于分布式开发
* 能够升任上千开发人员的规模add
* 性能优异
* 保持完整性和可靠性
* 强化责任（能够定位谁改动了文件，甚至改动的原因）
* 不可变性（Git版本库中存储的数据对象均为不可变的）
* 原子事务
* 支持并鼓励基于分支的开发
* 完整的版本库
* 一个清晰的内部设计
* 免费自由

## 安装 Git

### fedora install Git
```sh
dnf -y install git
```

### windows install Git

[Git Download](https://git-scm.com/downloads)

## 起步

Git 负责管理变化。鉴于这一意图，Git 与其他版本控制系统有许多共同点：
* 提交的概念
* 变更日志
* 版本库

当然，Git 还提供了许多新奇的事物。

## Git GUI 工具
### Windows 
安装 Git windows + [tortoisegit](https://tortoisegit.org/)

### Git 命令行

```hightlight
[breap@localhost ~]$ git --help
usage: git [--version] [--help] [-C <path>] [-c <name>=<value>]
           [--exec-path[=<path>]] [--html-path] [--man-path] [--info-path]
           [-p | --paginate | --no-pager] [--no-replace-objects] [--bare]
           [--git-dir=<path>] [--work-tree=<path>] [--namespace=<name>]
           <command> [<args>]

These are common Git commands used in various situations:

start a working area (see also: git help tutorial)
   clone      Clone a repository into a new directory
   init       Create an empty Git repository or reinitialize an existing one

work on the current change (see also: git help everyday)
   add        Add file contents to the index
   mv         Move or rename a file, a directory, or a symlink
   reset      Reset current HEAD to the specified state
   rm         Remove files from the working tree and from the index

examine the history and state (see also: git help revisions)
   bisect     Use binary search to find the commit that introduced a bug
   grep       Print lines matching a pattern
   log        Show commit logs
   show       Show various types of objects
   status     Show the working tree status

grow, mark and tweak your common history
   branch     List, create, or delete branches
   checkout   Switch branches or restore working tree files
   commit     Record changes to the repository
   diff       Show changes between commits, commit and working tree, etc
   merge      Join two or more development histories together
   rebase     Reapply commits on top of another base tip
   tag        Create, list, delete or verify a tag object signed with GPG

collaborate (see also: git help workflows)
   fetch      Download objects and refs from another repository
   pull       Fetch from and integrate with another repository or a local branch
   push       Update remote refs along with associated objects

'git help -a' and 'git help -g' list available subcommands and some
concept guides. See 'git help <command>' or 'git help <concept>'
to read about a specific subcommand or concept.
```
```sh
[breap@localhost ~]$ git --version
git version 2.17.1
```
从历史来开，Git 是作为一套简单的、独特的、独立的命令提供的，并按照 “UNIX工具包”的哲学来开发的：打造小的、可互操作的工具。

你可能需要使用双破折号分离并显式标识文件名，否则可能会误认为他们是命令的另一部分。
```sh
# checkout the tag named "main.c"
git checkout main.c
# checkout the file named "main.c"
git checkout -- main.c
```

### Git 使用快速入门

有两种建立 Git 版本库的基础技术：
* 从头开始创建，用现有的内容填充它
* 复制（或叫克隆） 一个已有的版本库。

从一个空的版本库开始比较简单，所以由此开始吧：

#### 创建初始化版本库

```sh
mkdir test
cd test
echo "Hello world" > hello
```

```sh
[breap@localhost test]$ git init
Initialized empty Git repository in /home/breap/test/.git/
```

#### 将文件添加到版本库中

```sh
git add hello
```

Git 将 ```add``` 和 ```commit``` 这两步分开，以避免频繁变换。试想一下，如果在你每一次添加，移除或更改一个文件的时候都要更新版本库，这是多么让人困惑、麻烦和费时阿！ 想反，多次临时的和相关的步骤，如一次添加，可以“批处理”，来保持版本库是稳定和一致的。

运行 ```git status``` 命令：
```sh
[breap@localhost test]$ git status
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)

	new file:   hello
```
这个命令显示新文件 hello 将在下一次提交的时候添加到版本库里。


一条完全限定的 ```git commit``` 命令必须提供日志消息和作者。

```sh
[breap@localhost test]$ git commit -m "init repo"
[master (root-commit) c646998] init repo
 Committer: breap <breap@localhost.localdomain>
Your name and email address were configured automatically based
on your username and hostname. Please check that they are accurate.
You can suppress this message by setting them explicitly:

    git config --global user.name "Your Name"
    git config --global user.email you@example.com

After doing this, you may fix the identity used for this commit with:

    git commit --amend --reset-author

 1 file changed, 1 insertion(+)
 create mode 100644 hello
```

可以在命令行中提供一条日志消息，但是更典型的做法是在交互式编辑器会话期间创建消息。这样，你将能够在你最喜欢的编辑器里面编写一条完整而且详细的日志消息。为了在 ```git commit``` 期间让 Git 打开你最爱的编辑器，要设置你的 ```GIT_EDITOR``` 环境变量。

```sh
# 在 bash 中
export GIT_EDITOR=vim
```

在把新文件提交到版本库后， ```git status``` 命令显示没有突出的、暂存的变更需要提交。

```sh
[breap@localhost test]$ git status
On branch master
nothing to commit, working tree clean
```

#### 配置提交作者

```sh
[breap@localhost test]$ git config user.name "Tester"
[breap@localhost test]$ git config user.email "Tester@example.com"
```

也可以使用 ```GIT_AUTHOR_NAME``` 和 ```GIT_AUTHOR_EMAIL``` 环境变量来设置 ```name ,email``` 。这些变量一旦设置就会覆盖所有的配置设置。

#### 再次提交    

修改一下文件
```sh
[breap@localhost test]$ echo "Modify" >> hello
[breap@localhost test]$ cat hello
Hello world
Modify
[breap@localhost test]$ git commit hello
[master d7b7aaa] Modify
 1 file changed, 1 insertion(+)
```

注：
这个文件已添加到版本库里了，没有必要再次使用 ```git add``` 把文件添加到版本库.

#### 查看提交
一旦在版本库中有了提交，就可以通过多种方式查看他们。

如 ：```git log``` 命令会产生版本库里一系列单独提交的历史：

```sh
[breap@localhost test]$ git log
commit d7b7aaab097379242654bd03423cdb543f57939b (HEAD -> master)
Author: Tester <Tester@example.com>
Date:   Sat Jun 30 14:13:56 2018 +0800

    Modify

commit c646998491f03467cab4a464d19dea73e34cd4bc
Author: breap <breap@localhost.localdomain>
Date:   Sat Jun 30 13:56:40 2018 +0800

    init repo
```

为了查看特定提交的更加详细的信息，可以使用 ```git show``` 命令带一个提交码。

```sh
[breap@localhost test]$ git show c646998491f03467cab4a464d19dea73e34cd4bc
commit c646998491f03467cab4a464d19dea73e34cd4bc
Author: breap <breap@localhost.localdomain>
Date:   Sat Jun 30 13:56:40 2018 +0800

    init repo

diff --git a/hello b/hello
new file mode 100644
index 0000000..802992c
--- /dev/null
+++ b/hello
@@ -0,0 +1 @@
+Hello world
```

另一种查看方式是使用 ```show-branch``` ,提供当前开发分支简洁的单行摘要。

```sh
[breap@localhost test]$ git show-branch --more=10
[master] Modify
[master^] init repo
```

#### 查看提交差异

```sh
[breap@localhost test]$ git diff c646998491f03467cab4a464d19dea73e34cd4bc d7b7aaab097379242654bd03423cdb543f57939b
diff --git a/hello b/hello
index 802992c..6129209 100644
--- a/hello
+++ b/hello
@@ -1 +1,2 @@
 Hello world
+Modify
```

#### 版本库内文件的删除和重命名

删除：
```sh
[breap@localhost test]$ git rm hello
rm 'hello'
[breap@localhost test]$ git commit -m "remove hello"
[master 45c1d1a] remove hello
 1 file changed, 2 deletions(-)
 delete mode 100644 hello
```

重命名：
```sh
# eg
git mv test hello
```

#### 创建版本库副本

可以使用 ```git clone``` 命令


### 配置文件

可以使用 ```git config``` 命令
Git 的配置文件全都是简单的 .ini 文件风格的文本文件。它们记录了很多 Git 命令使用的各种选项和设置。有的设置表示纯个人偏好（是否用到 color.paper?）;有些则是对版本库的正常运作非常重要（如 core.repositoryformatversion）;再之外的一些设置会稍微改变命令的行为（如 gc.auto）。

和很多工具一样，Git 支持不同层次的配置文件。按照优先级递减的顺序，如下所示：

* .git.config 版本库特定的配置设置，可用 ```--file``` 选项修改，是默认选项。这些设置拥有最高优先级。
* ~/.gitconfig 用户特定的配置设置，可用 ```--global``` 选项修改
* /etc/gitconfig 这是系统范围内的配置设置，如果你有他的 文件写权限，你就可以用 ```--system``` 选项修改它。

建立一个作者名和 email 地址，用于你对所有版本库的所有提交，可以用 ```git config --global``` 命令给在 ```$HOME/.gitconfig``` 文件里的 ```user.name``` 和 ```user.email``` 赋值。

使用 git config -l 列出在整组配置文件里共同查找的所有变量的设置值。

```sh
[breap@localhost test]$ git config -l
gui.recentrepo=/home/breap/blog
user.name=breap
user.email=dj8037152@gmail.com
core.repositoryformatversion=0
core.filemode=true
core.bare=false
core.logallrefupdates=true
user.name=Tester
user.email=Tester@example.com
```

可以使用 --unset 选项来移除设置：
```sh
git config --unset --global user.name
```

在编写提交日志信息的时候，编辑器的选择按照以下步骤的顺序确定：
* GIT_EDITOR 环境变量
* core.editor 环境变量
* VISUAL 环境变量
* EDITOR 环境变量
* vi 命令

#### 配置别名
对初学者来说，这有一个设置命令别名的提示。
```sh
git config --global alias.show-graph \
  'log --graph --abbrev-commit --pretty=oneline'
```

## 更改提交
提交记录你的工作历史记录，并且保证你所做的更改时神圣不可侵犯的，但该提交自身并不是一成不变的。 Git 提供了几个工具和命令，专门用来帮你修改完善版本库中的提交。有很多正当理由让你去修改或返工某个提交或整个提交序列：
* 可以在某个问题变为遗留问题之前修复它；
* 可以将大而全面的变更分解为一系列小而专题的提交。相反，也可以将一些小的变更组合成一个更大的提交；
* 可以合并反馈评论和建议；
* 可以在不破坏构建需求的情况下重新排列提交序列；
* 可以将提交调整为一个更合乎逻辑的序列；
* 可以删除意外提交的调试代码；

### 关于修改历史记录的注意事项
作为一般原则，只要没有其他开发人员已经获得了你的版本库的副本，你就可以自由地修改和完善版本库提交历史记录。或者更学术一点，只要没人有版本库中某个分支的副本，你就可以修改该分支。不过要记住一个概念，如果一个分支已经公开了，并且可能已经存在于其他版本库中了，那你就不应该重写、修改或更改该分支的任何部分。

### 使用 git reset
git reset 命令会把版本库和工作目录改变为已知状态。具体而言， git reset 调整 HEAD 引用指向给定的提交，默认情况下还会更新索引已匹配该提交。根据需要， git reset 命令也可以修改工作目录以呈现给定提交代表的项目修订版本。

可以把 git reset 当成 “破坏性的”，因为它可以覆盖并销毁工作目录中的修改。事实上，数据可能会丢失。即使你备份了文件，也可能无法恢复你的工作。然而，此命令的重点时为 HEAD 、索引和工作目录建立与恢复已知的状态。

git reset 命令主要有三个选项: --soft 、--mixed 、--hard。
* git reset --soft:会将 HEAD 引用指向给定提交。索引和工作目录的内容保持不变。这个版本的命令有 “最小” 影响，只改变一个符号引用的状态使其指向一个新提交。
* git reset --mixed:会将 HEAD 引用指向给定提交。索引内容也跟着改变以符合给定提交的树结构，但是工作目录中的内容保持不变。这个版本的命令将索引变成你刚刚暂存该提交全部变化时的状态，它会显示工作目录中还有什么修改。
注意，--mixed 是 git reset 的默认模式。
* git reset --hard:会将 HEAD 引用指向给定提交。索引的内容也跟着改变以符合给定提交的树结构。此外，工作目录的内容也随之改变以反映给定提交表示树的状态。
当改变工作目录的时候，整个目录结构都改成给定提交对应的样子。做的修改都将丢失，新文件将被删除。在给定提交中但不再工作目录中的文件将恢复过来。
git reset 选项影响如下表：

|   选项  | HEAD | 索引 | 工作目录 |
|:-------:|:----:|:----:|:--------:|
|  --soft |   Y  |   N  |     N    |
| --mixed |   Y  |  Y   |     N    |
|  --hard |   Y  |   Y  |     Y    |

#### revert a commit already pushed to a remote repository
[Git HowTo: revert a commit already pushed to a remote repository
](https://gist.github.com/gunjanpatel/18f9e4d1eb609597c50c2118e416e6a6)
eg:
```sh
git reset HEAD^ --hard

git push origin -f
#If forcing a push is failed，it might mean that the remote server is refusing
#non fast-forward pushes either via receive.denyNonFastForwards config variable.
#please read the next link;
```

[Git reset --hard and a remote repository
](https://stackoverflow.com/questions/1377845/git-reset-hard-and-a-remote-repository)

## git submodule

### add submodule
```git
git submodule add $git_rep
```
Using submodule
```git
git submodule init
git submodule update
```

### remove git submodule
```git
1. git submodule deinit -f -- $submodule_dir    
2. rm -rf .git/modules/$submodule_dir
3. git rm -f $submodule_dir
4. git commit
```

### Git submodule URL not including username?
1. Modify your .gitmodules and remove the username from the url:
```git
[submodule foo]
  path = sub/foo
  url = https://example.com/git/foo.git
```
2. `git submodule sync`

### 遇到的一些问题
* [Unable to find current origin/master revision in submodule path](https://stackoverflow.com/questions/40987847/unable-to-find-current-origin-master-revision-in-submodule-path)

## Link
* [How do I remove a submodule?](https://stackoverflow.com/questions/1260748/how-do-i-remove-a-submodule)
* [Git Submodules: Adding, Using, Removing, Updating](https://chrisjean.com/git-submodules-adding-using-removing-and-updating/)
* [TortoiseGit not showing icon overlays](https://stackoverflow.com/questions/25156238/tortoisegit-not-showing-icon-overlays)

---
- [上一级](README.md)
- 上一篇 -> [数据结构-图](graph.md)
- 下一篇 -> [多线程 VS 多进程](mulThreadAndMulProcesses.md)
