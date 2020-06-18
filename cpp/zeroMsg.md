# ZeroMQ


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [ZeroMQ](#zeromq)
  - [简介](#简介)
  - [Programming with ZeroMQ](#programming-with-zeromq)
  - [ZeroMQ Socket Types](#zeromq-socket-types)
  - [ZeroMQ guide](#zeromq-guide)
  - [构建](#构建)
    - [构建 libZmq](#构建-libzmq)
    - [构建 cppzmq](#构建-cppzmq)
  - [cppzmq 示例](#cppzmq-示例)
    - [inproc demo](#inproc-demo)
    - [request/reply demo](#requestreply-demo)
    - [Publish/Subscribe demo](#publishsubscribe-demo)
- [Link](#link)

<!-- /code_chunk_output -->


## 简介 
[ZeroMQ](https://zeromq.org/) (also spelled ØMQ, 0MQ or ZMQ) is a high-performance asynchronous messaging library, aimed at use in distributed or concurrent applications. It provides a message queue, but unlike message-oriented middleware, a ZeroMQ system can run without a dedicated message broker.

ZeroMQ supports common messaging patterns (pub/sub, request/reply, client/server and others) over a variety of transports (TCP, in-process, inter-process, multicast, WebSocket and more), making inter-process messaging as simple as inter-thread messaging. This keeps your code clear, modular and extremely easy to scale.

ZeroMQ is developed by a large community of contributors. There are third-party bindings for many popular programming languages and native ports for C# and Java.

## Programming with ZeroMQ
ZeroMQ as a library works through sockets by following certain network communication patterns. It is designed to work asynchronously, and that’s where the MQ suffix to its name comes - from thread queuing messages before sending them.

## ZeroMQ Socket Types
ZeroMQ differs in the way its sockets work. Unlike the synchronous way the regular sockets work, ZeroMQ’s socket implementation “present an abstraction of an asynchronous message queue”.

The way these sockets work depend on the type of socket chosen. And flow of messages being sent depend on the chosen patterns, of which there are four:

* Request/Reply Pattern: Used for sending a request and receiving subsequent replies for each one sent.
* Publish/Subscribe Pattern: Used for distributing data from a single process (e.g. publisher) to multiple recipients (e.g. subscribers).
* Pipeline Pattern: Used for distributing data to connected nodes.
* Exclusive Pair Pattern: Used for connecting two peers together, forming a pair.

## ZeroMQ guide
[ØMQ - The Guide](http://zguide.zeromq.org/) - 包含 zeroMsg 说明和多种编程语言的例子

## 构建
### 构建 libZmq 
1. 下载 [libzmq](https://github.com/zeromq/libzmq/releases) 最新版源码
2. 使用 cmake 生成 Visual studio solution 
    ```sh
    cd $libZmq_dir
    mkdir vs 
    cd vs
    cmake -G "Visual Studio 14 2015" ../
    ```
3. 打开生成的 ZeroMQ.sln ,生成相关的库

附注：使用的版本为 zeromq-4.3.2


### 构建 cppzmq
1. 下载 [cppzmq](https://github.com/zeromq/cppzmq/releases)
2. 以管理员权限打开 VisualStudio2015,打开上面生成的 libzmq 解决方案
3. 生成其中的 INSTALL 项目（将会安装 libzmq）
4. 使用 cmake 生成 Visual studio solution 
    ```sh
    cd $cppzmq_dir
    mkdir vs 
    cd vs
    # set -DCPPZMQ_BUILD_TESTS=OFF ,否则会出现 - Unknown CMake command "catch_discover_tests".
    cmake -G "Visual Studio 14 2015" ../ -DCPPZMQ_BUILD_TESTS=OFF
    ```
5. 打开生成的 ZeroMQ.sln ,生成相关的库

附注：使用的版本为 cppzmq-4.6.0

## cppzmq 示例
### inproc demo
```c++
#include <iostream>
#include <zmq_addon.hpp>

int main()
{
	zmq::context_t ctx;
	zmq::socket_t sock1(ctx, zmq::socket_type::pair);
	zmq::socket_t sock2(ctx, zmq::socket_type::pair);
	sock1.bind("inproc://test");
	sock2.connect("inproc://test");

	std::array<zmq::const_buffer, 2> send_msgs = {
		zmq::str_buffer("foo"),
		zmq::str_buffer("bar!")
	};
	if (!zmq::send_multipart(sock1, send_msgs))
		return 1;

	std::vector<zmq::message_t> recv_msgs;
	const auto ret = zmq::recv_multipart(
		sock2, std::back_inserter(recv_msgs));
	if (!ret)
		return 1;
	std::cout << "Got " << *ret
		<< " messages" << std::endl;

	for (auto i=0u;i<recv_msgs.size();++i)
	{
		std::cout << "Index "<< i <<" "<<recv_msgs[i].to_string()<<std::endl;
	}

	return 0;
}
```

### request/reply demo
server:
```c++
#include <iostream>
#include <zmq_addon.hpp>

int main()
{
	zmq::context_t ctx;
	zmq::socket_t server(ctx, zmq::socket_type::rep);
	server.bind("tcp://*:9999");

	while (true)
	{
		std::vector<zmq::message_t> recv_msgs;
		const auto ret = zmq::recv_multipart(
			server, std::back_inserter(recv_msgs));
		if (!ret)
			return 1;
		std::cout << "Got " << *ret
			<< " messages" << std::endl;


		for (auto i = 0u; i < recv_msgs.size(); ++i)
		{
			std::cout << "Index " << i << " " << recv_msgs[i].to_string() << std::endl;
		}
		server.send(zmq::str_buffer("")); // # fixing for recv-send pair
	}
	

	system("pause");

	return 0;
}
```

client:
```c++
#include <iostream>
#include <zmq_addon.hpp>
#include "zmq.hpp"
#include <exception>
#include <thread>
#include <chrono>

int main()
{
	zmq::context_t ctx;
	zmq::socket_t client(ctx, zmq::socket_type::req);
	client.connect("tcp://localhost:9999");

	std::array<zmq::const_buffer, 2> send_msgs = {
		zmq::str_buffer("foo"),
		zmq::str_buffer("bar!")
	};
	while (true)
	{
		try
		{
			if (!zmq::send_multipart(client, send_msgs))
				return 1;
		}
		catch (std::exception& e)
		{
			std::cout << e.what()<<std::endl;
		}
		zmq::mutable_buffer buf;
		client.recv(buf); // # fixing for recv-send pair
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}
```
The client creates a socket of type request, connects and starts sending messages.

Both the send and receive methods are blocking (by default). For the receive it is simple: if there are no messages the method will block. For sending it is more complicated and depends on the socket type. For request sockets, if the high watermark is reached or no peer is connected the method will block.

### Publish/Subscribe demo

# Link 
* [Unknown CMake command "catch_discover_tests".](https://github.com/zeromq/cppzmq/issues/334)
* [详解报错[zmq.error.ZMQError: Operation cannot be accomplished in current state]](https://blog.csdn.net/ybdesire/article/details/81435291)
* [How To Work with the ZeroMQ Messaging Library](https://www.digitalocean.com/community/tutorials/how-to-work-with-the-zeromq-messaging-library#zeromq-transport-types)