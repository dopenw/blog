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

## dockerfile


Dockerfile 中每一个指令都会建立一层, RUN 也不例外。每一个RUN 的行为,就和刚才我们手工建立镜像的过程一样:新建立一层在其上执行这些命令,执行结束后, commit 这一层的修改,构成新的镜像。

```docker
FROM debian:jessie
RUN apt-get update
RUN apt-get install -y gcc libc6-dev make
RUN wget -O redis.tar.gz "http://download.redis.io/releases/redi
s-3.2.5.tar.gz"
RUN mkdir -p /usr/src/redis
RUN tar -xzf redis.tar.gz -C /usr/src/redis --strip-components=1
RUN make -C /usr/src/redis
RUN make -C /usr/src/redis install
```
这种写法，创建了7册镜像。

在 Dockerfile 文件所在目录执行:
```
$ docker build -t nginx:v3 .
Sending build context to Docker daemon 2.048 kB
Step 1 : FROM nginx
---> e43d811ce2f4
Step 2 : RUN echo '<h1>Hello, Docker!</h1>' > /usr/share/nginx/h
tml/index.html
---> Running in 9cdc27646c7b
---> 44aa4490ce2c
Removing intermediate container 9cdc27646c7b
Successfully built 44aa4490ce2c
```
### 镜像构建上下文
当构建的时候,用户会指定构建镜像上下文的路径, docker build 命令得知这个路径后,会将路径下的所有内容打包,然后上给Docker 引擎。这样 Docker 引擎收到这个上下文包后,展开就会获得构建镜像所需的一切文件。

从标准输入中读取 Dockerfile 进行构建
```
docker build - < Dockerfile
```
或
```
cat Dockerfile | docker build -
````
### dockerfile指令

#### copy 复制文件
* COPY <源路径>... <目标路径>
* COPY ["<源路径1>",... "<目标路径>"]

COPY 指令将从构建上下文目录中 <源路径> 的文件/目录复制到新的一层的镜像内的 <目标路径> 位置。比如:

```docker
COPY package.json /usr/src/app/
```

#### ADD 更高级的复制文件(尽可能使用copy，不推荐使用)
ADD 指令和 COPY 的格式和性质基本一致。但是在 COPY 基础上增加了一些功能。
比如 <源路径> 可以是一个 URL ,这种情况下,Docker 引擎会试图去下载这个链接的文件放到 <目标路径> 去。下载后的文件权限自动设置为 600 ,如果这并不是想要的权限,那么还需要增加额外的一层 RUN 进行权限调整,另外,如果下载的是个压缩包,需要解压缩,也一样还需要额外的一层 RUN 指令进行解压缩。所以不如直接使用 RUN 指令,然后使用 wget 或者 curl 工具下载,处理权限、解压缩、然后清理无用文件更合理。因此,这个功能其实并不实用,而且不推荐使用。

如果 <源路径> 为一个 tar 压缩文件的话,压缩格式为 gzip , bzip2 以及xz 的情况下, ADD 指令将会自动解压缩这个压缩文件到 <目标路径> 去。

在某些情况下,这个自动解压缩的功能非常有用,比如官方镜像 ubuntu 中:

```
FROM scratch
ADD ubuntu-xenial-core-cloudimg-amd64-root.tar.gz /
...
```

在 Docker 官方的最佳实践文档中要求,尽可能的使用 COPY ,因为 COPY 的语义很明确,就是复制文件而已,而 ADD 则包含了更复杂的功能,其行为也不一定很清晰。最适合使用 ADD 的场合,就是所提及的需要自动解压缩的场合。

#### CMD 容器启动命令
CMD 指令的格式和 RUN 相似，也是两种格式：
* shell 格式： CMD <命令>
* exec 格式： CMD ["可执行文件", "参数1", "参数2"...]
参数列表格式： CMD ["参数1", "参数2"...] 。在指定了 ENTRYPOINT 指令后，用 CMD 指定具体的参数。之前介绍容器的时候曾经说过，Docker 不是虚拟机，容器就是进程。既然是进程，那么在启动容器的时候，需要指定所运行的程序及参数。 CMD 指令就是用于指定默认的容器主进程的启动命令的。

提到 CMD 就不得不提容器中应用在前台执行和后台执行的问题。这是初学者常出现的一个混淆。
CMD 容器启动命令Docker 不是虚拟机，容器中的应用都应该以前台执行，而不是像虚拟机、物理机里面那样，用 upstart/systemd 去启动后台服务，容器内没有后台服务的概念。

error
```docker
CMD service nginx start
```
然后发现容器执行后就立即退出了。甚至在容器内去使用 systemctl 命令结果却发现根本执行不了。这就是因为没有搞明白前台、后台的概念，没有区分容器和虚拟机的差异，依旧在以传统虚拟机的角度去理解容器。
对于容器而言，其启动程序就是容器应用进程，容器就是为了主进程而存在的，主进程退出，容器就失去了存在的意义，从而退出，其它辅助进程不是它需要关心的东西。而使用 service nginx start 命令，则是希望 upstart 来以后台守护进程形式启动 nginx 服务。而刚才说了 CMD service nginx start 会被理解为 CMD [
"sh", "-c", "service nginx start"] ，因此主进程实际上是 sh 。那么当service nginx start 命令结束后， sh 也就结束了， sh 作为主进程退出了，自然就会令容器退出。

正确的做法是直接执行 nginx 可执行文件，并且要求以前台形式运行。比如：
```docker
CMD ["nginx", "-g", "daemon off;"]
```

#### ENTRYPOINT 入口点
ENTRYPOINT 的格式和 RUN 指令格式一样，分为 exec 格式和 shell 格式。
ENTRYPOINT 的目的和 CMD 一样，都是在指定容器启动程序及参
数。 ENTRYPOINT 在运行时也可以替代，不过比 CMD 要略显繁琐，需要通过docker run 的参数 --entrypoint 来指定。
当指定了 ENTRYPOINT 后， CMD 的含义就发生了改变，不再是直接的运行其命令，而是将 CMD 的内容作为参数传给 ENTRYPOINT 指令，换句话说实际执行时，将变为：
```
<ENTRYPOINT> "<CMD>"
```

#### ENV 设置环境变量
格式有两种：
* ENV <key> <value>
* ENV <key1>=<value1> <key2>=<value2>...
这个指令很简单，就是设置环境变量而已，无论是后面的其它指令，如 RUN ，还是运行时的应用，都可以直接使用这里定义的环境变量。

```
ENV VERSION=1.0 DEBUG=on \
NAME="Happy Feet"
```

#### ARG 构建参数
格式：
* ARG <参数名>[=<默认值>]
构建参数和 ENV 的效果一样，都是设置环境变量。所不同的是， ARG 所设置的构建环境的环境变量，在将来容器运行时是不会存在这些环境变量的。但是不要因此就使用 ARG 保存密码之类的信息，因为 docker history 还是可以看到所有值的。

#### VOLUME 定义匿名卷
格式为：
* VOLUME ["<路径1>", "<路径2>"...]
* VOLUME <路径>
之前我们说过，容器运行时应该尽量保持容器存储层不发生写操作，对于数据库类需要保存动态数据的应用，其数据库文件应该保存于卷(volume)中，后面的章节我们会进一步介绍 Docker 卷的概念。为了防止运行时用户忘记将动态文件所保存目录挂载为卷，在 Dockerfile 中，我们可以事先指定某些目录挂载为匿名卷，这样在运行时如果用户不指定挂载，其应用也可以正常运行，不会向容器存储层写入大量数据。

```docker
VOLUME /data
```

这里的 /data 目录就会在运行时自动挂载为匿名卷，任何向 /data 中写入的信息都不会记录进容器存储层，从而保证了容器存储层的无状态化。当然，运行时可以覆盖这个挂载设置。比如：

```docker
docker run -d -v mydata:/data xxxx
```

#### EXPOSE 声明端口
格式为
* EXPOSE <端口1> [<端口2>...] 。
EXPOSE 指令是声明运行时容器提供服务端口，这只是一个声明，在运行时并不会因为这个声明应用就会开启这个端口的服务。在 Dockerfile 中写入这样的声明有两个好处，一个是帮助镜像使用者理解这个镜像服务的守护端口，以方便配置映射；另一个用处则是在运行时使用随机端口映射时，也就是 docker run -P时，会自动随机映射 EXPOSE 的端口。

#### WORKDIR 指定工作目录
格式为
* WORKDIR <工作目录路径> 。
使用 WORKDIR 指令可以来指定工作目录（或者称为当前目录），以后各层的当前目录就被改为指定的目录，如该目录不存在， WORKDIR 会帮你建立目录。

之前提到一些初学者常犯的错误是把 Dockerfile 等同于 Shell 脚本来书写，这种错误的理解还可能会导致出现下面这样的错误：

error:
```docker
RUN cd /app
RUN echo "hello" > world.txt
```

#### USER 指定当前用户
格式：
* USER <用户名>
USER 指令和 WORKDIR 相似，都是改变环境状态并影响以后的层。 WORKDIR是改变工作目录，USER则是改变之后层的执行RUN, CMD 以及ENTRYPOINT 这类命令的身份。

当然，和WORKDIR一样， USER只是帮助你切换到指定用户而已，这个用户必须是事先建立好的，否则无法切换。

```docker
RUN groupadd -r redis && useradd -r -g redis redis
USER redis
RUN [ "redis-server" ]
```

#### HEALTHCHECK 健康检查
格式：
* HEALTHCHECK [选项] CMD <命令> ：设置检查容器健康状况的命令
* HEALTHCHECK NONE ：如果基础镜像有健康检查指令，使用这行可以屏蔽掉其健康检查指令
* HEALTHCHECK 指令是告诉 Docker 应该如何进行判断容器的状态是否正常，这是Docker 1.12 引入的新指令。
在没有 HEALTHCHECK 指令前，Docker 引擎只可以通过容器内主进程是否退出来判断容器是否状态异常。很多情况下这没问题，但是如果程序进入死锁状态，或者死循环状态，应用进程并不退出，但是该容器已经无法提供服务了。在 1.12 以前，Docker 不会检测到容器的这种状态，从而不会重新调度，导致可能会有部分容器已经无法提供服务了却还在接受用户请求。而自 1.12 之后，Docker 提供了 HEALTHCHECK 指令，通过该指令指定一行命令，用这行命令来判断容器主进程的服务状态是否还正常，从而比较真实的反应容器实际状态。

#### ONBUILD 为他人做嫁衣裳
格式：
* ONBUILD <其它指令> 。
ONBUILD 是一个特殊的指令，它后面跟的是其它指令，比如 RUN , COPY 等，而这些指令，在当前镜像构建时并不会被执行。只有当以当前镜像为基础镜像，去构建下一级镜像的时候才会被执行。

### 其它生成镜像的方法

#### 从 rootfs 压缩包导入
格式：
* docker import [选项] <文件>|<URL>|- [<仓库名>[:<标签>]]

压缩包可以是本地文件、远程 Web 文件，甚至是从标准输入中得到。压缩包将会在镜像 / 目录展开，并直接作为镜像第一层提交。

#### 保存镜像
使用 docker save 命令可以将镜像保存为归档文件。


#### 加载镜像

docker load

## CentOS/RHEL 的用户需要注意的事项

在 Ubuntu/Debian 上有 UnionFS 可以使用，如 aufs 或者 overlay2 ，而CentOS 和 RHEL 的内核中没有相关驱动。因此对于这类系统，一般使用devicemapper 驱动利用 LVM 的一些机制来模拟分层存储。这样的做法除了性能比较差外，稳定性一般也不好，而且配置相对复杂。Docker 安装在 CentOS/RHEL上后，会默认选择 devicemapper ，但是为了简化配置，其 devicemapper 是跑在一个稀疏文件模拟的块设备上，也被称为 loop-lvm 。这样的选择是因为不需要额外配置就可以运行 Docker，这是自动配置唯一能做到的事情。但是 looplvm的做法非常不好，其稳定性、性能更差，无论是日志还是 docker info 中都会看到警告信息。官方文档有明确的文章讲解了如何配置块设备给devicemapper 驱动做存储层的做法，这类做法也被称为配置 direct-lvm 。删除本地镜像除了前面说到的问题外， devicemapper + loop-lvm 还有一个缺陷，因为它是稀疏文件，所以它会不断增长。用户在使用过程中会注意到/var/lib/docker/devicemapper/devicemapper/data 不断增长，而且无法控制。很多人会希望删除镜像或者可以解决这个问题，结果发现效果并不明显。原因就是这个稀疏文件的空间释放后基本不进行垃圾回收的问题。因此往往会出现即使删除了文件内容，空间却无法回收，随着使用这个稀疏文件一直在不断增长。所以对于 CentOS/RHEL 的用户来说，在没有办法使用 UnionFS 的情况下，一定要配置 direct-lvm 给 devicemapper ，无论是为了性能、稳定性还是空间利用率。

或许有人注意到了 CentOS 7 中存在被 backports 回来的 overlay 驱动，不过CentOS 里的这个驱动达不到生产环境使用的稳定程度，所以不推荐使用。

## 镜像的实现原理
Docker 镜像是怎么实现增量的修改和维护的？ 每个镜像都由很多层次构成，Docker 使用 Union FS 将这些不同的层结合到一个镜像中去。

通常 Union FS 有两个用途, 一方面可以实现不借助 LVM、RAID 将多个disk挂到同一个目录下,另一个更常用的就是将一个只读的分支和一个可写的分支联合在一起，Live CD正是基于此方法可以允许在镜像不变的基础上允许用户在其上进行一些写操作。 Docker 在AUFS上构建的容器也是利用了类似的原理。

##容器操作

### 进入容器

在使用 -d 参数时，容器启动后会进入后台。 某些时候需要进入容器进行操作，有很多种方法，包括使用 docker attach 命令或 nsenter 工具等。

### attach 命令
docker attach 是Docker自带的命令。下面示例如何使用该命令。
```docker
$ sudo docker run -idt ubuntu
243c32535da7d142fb0e6df616a3c3ada0b8ab417937c853a9e1c251f499f550
$ sudo docker ps
CONTAINER ID IMAGE COMMAND CREA
TED STATUS PORTS NAMES
243c32535da7 ubuntu:latest "/bin/bash" 18 s
econds ago Up 17 seconds nostalgi
c_hypatia
$sudo docker attach nostalgic_hypatia
root@243c32535da7:/#
```
但是使用 attach 命令有时候并不方便。当多个窗口同时 attach 到同一个容器的时候，所有窗口都会同步显示。当某个窗口因命令阻塞时,其他窗口也无法执行操作了。

### 导出和导入容器
#### 导出容器
如果要导出本地某个容器，可以使用 docker export 命令。

#### 导入容器快照
可以使用 docker import 从容器快照文件中再导入为镜像。


[上一级](base.md)
[下一篇](google_search_tips.md)
