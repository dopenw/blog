# docker 使用记录
[toc]

## 基本概念
* image(镜像)
* container（容器）
* repository（仓库）


### Docker 镜像

我们都知道，操作系统分为内核和用户空间。对于 Linux 而言，内核启动后，会挂
载 root 文件系统为其提供用户空间支持。而 Docker 镜像（Image），就相当于
是一个 root 文件系统。比如官方镜像 ubuntu:14.04 就包含了完整的一套
Ubuntu 14.04 最小系统的 root 文件系统。
Docker 镜像是一个特殊的文件系统，除了提供容器运行时所需的程序、库、资
源、配置等文件外，还包含了一些为运行时准备的一些配置参数（如匿名卷、环境
变量、用户等）。镜像不包含任何动态数据，其内容在构建之后也不会被改变。
#### 分层存储
因为镜像包含操作系统完整的 root 文件系统，其体积往往是庞大的，因此在
Docker 设计时，就充分利用 Union FS 的技术，将其设计为分层存储的架构。所以
严格来说，镜像并非是像一个 ISO 那样的打包文件，镜像只是一个虚拟的概念，其
实际体现并非由一个文件组成，而是由一组文件系统组成，或者说，由多层文件系
统联合组成。
镜像构建时，会一层层构建，前一层是后一层的基础。每一层构建完就不会再发生
改变，后一层上的任何改变只发生在自己这一层。比如，删除前一层文件的操作，
实际不是真的删除前一层的文件，而是仅在当前层标记为该文件已删除。在最终容
器运行的时候，虽然不会看到这个文件，但是实际上该文件会一直跟随镜像。因
此，在构建镜像的时候，需要额外小心，每一层尽量只包含该层需要添加的东西，
任何额外的东西应该在该层构建结束前清理掉。
分层存储的特征还使得镜像的复用、定制变的更为容易。甚至可以用之前构建好的
镜像作为基础层，然后进一步添加新的层，以定制自己所需的内容，构建新的镜
像。


### Docker 容器
镜像（Image）和容器（Container）的关系，就像是面向对象程序设计中
的 类 和 实例 一样，镜像是静态的定义，容器是镜像运行时的实体。容器可以被
创建、启动、停止、删除、暂停等。
容器的实质是进程，但与直接在宿主执行的进程不同，容器进程运行于属于自己的
独立的 命名空间。因此容器可以拥有自己的 root 文件系统、自己的网络配置、
自己的进程空间，甚至自己的用户 ID 空间。容器内的进程是运行在一个隔离的环
境里，使用起来，就好像是在一个独立于宿主的系统下操作一样。这种特性使得容
器封装的应用比直接在宿主运行更加安全。也因为这种隔离的特性，很多人初学
Docker 时常常会把容器和虚拟机搞混。
容器存储层的生存周期和容器一样，容器消亡时，容器存储层也随之消亡。因此，
任何保存于容器存储层的信息都会随容器删除而丢失。
按照 Docker 最佳实践的要求，容器不应该向其存储层内写入任何数据，容器存储
层要保持无状态化。所有的文件写入操作，都应该使用 数据卷（Volume）、或者
绑定宿主目录，在这些位置的读写会跳过容器存储层，直接对宿主(或网络存储)发
生读写，其性能和稳定性更高。
数据卷的生存周期独立于容器，容器消亡，数据卷不会消亡。因此，使用数据卷
后，容器可以随意删除、重新 run ，数据却不会丢失。

### Docker Registry
镜像构建完成后,可以很容易的在当前宿主上运行,但是,如果需要在其它服务器
上使用这个镜像,我们就需要一个集中的存储、分发镜像的服务,Docker Registry
就是这样的服务。
一个 Docker Registry 中可以包含多个仓库(Repository);每个仓库可以包含多
个标签(Tag);每个标签对应一个镜像。
通常,一个仓库会包含同一个软件不同版本的镜像,而标签就常用于对应该软件的
各个版本。我们可以通过 <仓库名>:<标签> 的格式来指定具体是这个软件哪个版
本的镜像。如果不给出标签,将以 latest 作为默认标签。
以 Ubuntu 镜像 为例, ubuntu 是仓库的名字,其内包含有不同的版本标签,
如, 14.04 , 16.04 。我们可以通过 ubuntu:14.04 ,或者 ubuntu:16.04
来具体指定所需哪个版本的镜像。如果忽略了标签,比如 ubuntu ,那将视为
ubuntu:latest 。
仓库名经常以 两段式路径 形式出现,比如 jwilder/nginx-proxy ,前者往往意
味着 Docker Registry 多用户环境下的用户名,后者则往往是对应的软件名。但这
并非绝对,取决于所使用的具体 Docker Registry 的软件或服务。
Docker Registry 公开服务
Docker Registry 公开服务是开放给用户使用、允许用户管理镜像的 Registry 服
务。一般这类公开服务允许用户免费上传、下载公开的镜像,并可能提供收费服务
供用户管理私有镜像。
最常使用的 Registry 公开服务是官方的 Docker Hub,这也是默认的 Registry,并
拥有大量的高质量的官方镜像。除此以外,还有 CoreOS 的 Quay.io,CoreOS 相
关的镜像存储在这里;Google 的 Google Container Registry,Kubernetes 的镜像
使用的就是这个服务。
由于某些原因,在国内访问这些服务可能会比较慢。国内的一些云服务商提供了针
对 Docker Hub 的镜像服务(Registry Mirror),这些镜像服务被称为加速器。常见
的有 阿里云加速器、DaoCloud 加速器、灵雀云加速器等。使用加速器会直接从国内的地址下载 Docker Hub 的镜像,比直接从官方网站下载速度会提高很多。
国内也有一些云服务商提供类似于 Docker Hub 的公开服务。比如 时速云镜像仓库、网易云镜像服务、DaoCloud 镜像市场、阿里云镜像库等。
#### 私有 Docker Registry
除了使用公开服务外,用户还可以在本地搭建私有 Docker Registry。Docker 官方
提供了 Docker Registry 镜像,可以直接使用做为私有 Registry 服务。在后续的相
关章节中,会有进一步的搭建私有 Registry 服务的讲解。
开源的 Docker Registry 镜像只提供了 Docker Registry API 的服务端实现,足以支
持 docker 命令,不影响使用。但不包含图形界面,以及镜像维护、用户管理、
访问控制等高级功能。在官方的商业化版本 Docker Trusted Registry 中,提供了这
些高级功能。
除了官方的 Docker Registry 外,还有第三方软件实现了 Docker Registry API,甚
至提供了用户界面以及一些高级功能。比如,VMWare Harbor 和 Sonatype
Nexus。

## Manage Docker as a non-root user

The docker daemon binds to a Unix socket instead of a TCP port. By default that Unix socket is owned by the user root and other users can only access it using sudo. The docker daemon always runs as the root user.

If you don’t want to use sudo when you use the docker command, create a Unix group called docker and add users to it. When the docker daemon starts, it makes the ownership of the Unix socket read/writable by the docker group.

1. Create the docker group.

```
$ sudo groupadd docker
```
2. Add your user to the docker group.

```
$ sudo usermod -aG docker $USER
```

3. reboot system

4. Verify that you can run docker commands without sudo.

```
$ docker run hello-world
```
This command downloads a test image and runs it in a container. When the container runs, it prints an informational message and exits.
[source link](https://docs.docker.com/engine/installation/linux/linux-postinstall/)

## docker commit
当我们运行一个容器的时候(如果不使用卷的话),我们做的任何文件修
改都会被记录于容器存储层里。而 Docker 提供了一个 docker commit 命令,可
以将容器的存储层保存下来成为镜像。换句话说,就是在原有镜像的基础上,再叠
加上容器的存储层,并构成新的镜像。以后我们运行这个新镜像的时候,就会拥有
原有容器最后的文件变化。

docker commit 的语法格式为:
```
docker commit [选项] <容器ID或容器名> [<仓库名>[:<标签>]]
```
我们可以用下面的命令将容器保存为镜像(eg):
```
$ docker commit \
--author "Tao Wang <twang2218@gmail.com>" \
--message "修改了默认网页" \
webserver \
nginx:v2
sha256:07e33465974800ce65751acc279adc6ed2dc5ed4e0838f8b86f0c87aa
1795214
```

### 慎用 docker commit
使用 docker commit 命令虽然可以比较直观的帮助理解镜像分层存储的概念,但是实际环境中并不会这样使用。
首先,如果仔细观察之前的 docker diff $name的结果,你会发现除了真正想要修改的文件外,由于命令的执行,还有很多文件被改动或添加了。这还仅仅是最简单的操作,如果是安装软件包、编译构建,那会有大量的无关内容被添加进来,如果不小心清理,将会导致镜像极为臃肿。
此外,使用 docker commit 意味着所有对镜像的操作都是黑箱操作,生成的镜像也被称为黑箱镜像,换句话说,就是除了制作镜像的人知道执行过什么命令、怎么生成的镜像,别人根本无从得知。而且,即使是这个制作镜像的人,过一段时间后也无法记清具体在操作的。虽然 docker diff 或许可以告诉得到一些线索,但是远远不到可以确保生成一致镜像的地步。这种黑箱镜像的维护工作是非常痛苦
的。
而且,回顾之前提及的镜像所使用的分层存储的概念,除当前层外,之前的每一层都是不会发生改变的,换句话说,任何修改的结果仅仅是在当前层进行标记、添加、修改,而不会改动上一层。如果使用 docker commit 制作镜像,以及后期修改的话,每一次修改都会让镜像更加臃肿一次,所删除的上一层的东西并不会丢失,会一直如影随形的跟着这个镜像,即使根本无法访问到TM。这会让镜像更加臃肿。

### docker commit 用途
docker commit 命令除了学习之外,还有一些特殊的应用场合,比如被入侵后保存现场等。但是,不要使用 docker commit 定制镜像,定制行为应该使用Dockerfile 来完成。

[上一级](base.md)
[下一篇](google_search_tips.md)
