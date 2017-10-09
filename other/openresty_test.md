# openresty测试
[TOC]

## 1.ab压力测试

[原文链接](http://www.jianshu.com/p/43d04d8baaf7)

### 1.1压力测试的概念


#### 吞吐率（Requests per second）

概念：服务器并发处理能力的量化描述，单位是reqs/s，指的是某个并发用户数下单位时间内处理的请求数。某个并发用户数下单位时间内能处理的最大请求数，称之为最大吞吐率。
计算公式：总请求数 / 处理完成这些请求数所花费的时间，即
Request per second = Complete requests / Time taken for tests

#### 并发连接数（The number of concurrent connections）

概念：某个时刻服务器所接受的请求数目，简单的讲，就是一个会话。

#### 并发用户数（The number of concurrent users，Concurrency Level）

概念：要注意区分这个概念和并发连接数之间的区别，一个用户可能同时会产生多个会话，也即连接数。

#### 用户平均请求等待时间（Time per request）

计算公式：处理完成所有请求数所花费的时间/ （总请求数 / 并发用户数），即
Time per request = Time taken for tests /（ Complete requests / Concurrency Level）

#### 服务器平均请求等待时间（Time per request: across all concurrent requests）

计算公式：处理完成所有请求数所花费的时间 / 总请求数，即
Time taken for / testsComplete requests
可以看到，它是吞吐率的倒数。
同时，它也=用户平均请求等待时间/并发用户数，即
Time per request / Concurrency Level


### 1.2 ab工具简介

```
ab tools:
ab is a tool for benchmarking your Apache Hypertext Transfer Protocol (HTTP) server. It is designed to give you an impression of how your current Apache installation performs. This especially shows you how many requests per second your Apache installation is capable of serving.
```

### 1.3 ab工具使用
json.txt文件内容：
```
{"header": {"send_tm": "20170915","type": 0},"invalid_data": {"name": "fsd","expire_tm": 3,"cond": {"data": {"filed": "tt_filed","start_tm": "%lastmonth_3","end_tm": "%lastmonth_1"},"tname": "test"},"conf": {"driver": "mysql","db": {}}}}

```

用ab工具发送post请求到invaild

```
ab -p json.txt -T application/json -c 1000 -n 80000 http://127.0.0.1:6699/invalid
```

```
-c concurrency
        Number of multiple requests to perform at a time. Default is one request at a time.

-n requests
         Number of requests to perform for the benchmarking session.  The default  is to just perform a single request which usually leads to non-representative benchmarking results.

-T content-type
         Content-type header to use for POST/PUT data, eg. application/x-
          www-form-urlencoded. Default is text/plain.

-p POST-file
          File containing data to POST. Remember to also set -T.
```

### 1.4 测试结果
ngix_conf 首部：

```
worker_processes 1; 			#nginx worker 数量
error_log logs/error.log; 		#指定错误日志文件路径
error_log logs/debug.log debug;

events {
	worker_connections 1024;
}
```


#### 传递正常的json：
nginx_conf : worker_processes 1:

```
ab -p json.txt -T application/json -c 10 -n 8000 http://127.0.0.1:6699/invalid
```

```
#show :
Concurrency Level:      10
Time taken for tests:   1.101 seconds
Complete requests:      8000
Failed requests:        6977
   (Connect: 0, Receive: 0, Length: 6977, Exceptions: 0)
Total transferred:      1885402 bytes
Total body sent:        3056000
HTML transferred:       693402 bytes
Requests per second:    7268.50 [#/sec] (mean)
Time per request:       1.376 [ms] (mean)
Time per request:       0.138 [ms] (mean, across all concurrent requests)
Transfer rate:          1672.86 [Kbytes/sec] received
                        2711.49 kb/s sent
                        4384.35 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       1
Processing:     0    1   0.5      1      12
Waiting:        0    1   0.5      1      12
Total:          1    1   0.5      1      12

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      2
  90%      2
  95%      2
  98%      2
  99%      2
 100%     12 (longest request)
```

```
ab -p json.txt -T application/json -c 100 -n 8000 http://127.0.0.1:6699/invalid
```


```
Document Path:          /invalid
Document Length:        64 bytes

Concurrency Level:      100
Time taken for tests:   1.139 seconds
Complete requests:      8000
Failed requests:        6977
   (Connect: 0, Receive: 0, Length: 6977, Exceptions: 0)
Total transferred:      1885402 bytes
Total body sent:        3056000
HTML transferred:       693402 bytes
Requests per second:    7023.99 [#/sec] (mean)
Time per request:       14.237 [ms] (mean)
Time per request:       0.142 [ms] (mean, across all concurrent requests)
Transfer rate:          1616.58 [Kbytes/sec] received
                        2620.28 kb/s sent
                        4236.86 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.6      0       8
Processing:     2   14   2.6     15      20
Waiting:        2   14   2.6     15      20
Total:          9   14   2.3     15      20

Percentage of the requests served within a certain time (ms)
  50%     15
  66%     15
  75%     16
  80%     16
  90%     17
  95%     17
  98%     18
  99%     18
 100%     20 (longest request)

```

```
ab -p json.txt -T application/json -c 100 -n 1000 http://127.0.0.1:6699/invalid

```

```
Document Path:          /invalid
Document Length:        64 bytes

Concurrency Level:      100
Time taken for tests:   0.157 seconds
Complete requests:      1000
Failed requests:        0
Total transferred:      213000 bytes
Total body sent:        382000
HTML transferred:       64000 bytes
Requests per second:    6358.86 [#/sec] (mean)
Time per request:       15.726 [ms] (mean)
Time per request:       0.157 [ms] (mean, across all concurrent requests)
Transfer rate:          1322.69 [Kbytes/sec] received
                        2372.15 kb/s sent
                        3694.84 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   1.0      0       5
Processing:     2   14   2.5     15      19
Waiting:        2   14   2.5     15      19
Total:          7   15   1.9     15      20

Percentage of the requests served within a certain time (ms)
  50%     15
  66%     15
  75%     16
  80%     16
  90%     16
  95%     18
  98%     19
  99%     19
 100%     20 (longest request)

```


```
ab -p json.txt -T application/json -c 100 -n 2000 http://127.0.0.1:6699/invalid
```

```
Document Path:          /invalid
Document Length:        64 bytes

Concurrency Level:      100
Time taken for tests:   0.276 seconds
Complete requests:      2000
Failed requests:        977
   (Connect: 0, Receive: 0, Length: 977, Exceptions: 0)
Total transferred:      451402 bytes
Total body sent:        764000
HTML transferred:       153402 bytes
Requests per second:    7256.39 [#/sec] (mean)
Time per request:       13.781 [ms] (mean)
Time per request:       0.138 [ms] (mean, across all concurrent requests)
Transfer rate:          1599.39 [Kbytes/sec] received
                        2706.98 kb/s sent
                        4306.37 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.7      0       6
Processing:     2   13   2.7     13      19
Waiting:        2   13   2.8     13      19
Total:          7   13   2.4     13      19

Percentage of the requests served within a certain time (ms)
  50%     13
  66%     14
  75%     15
  80%     15
  90%     16
  95%     17
  98%     18
  99%     19
 100%     19 (longest request)
```

```
ab -p json.txt -T application/json -c 1000 -n 2000 http://127.0.0.1:6699/invalid
```

```
Document Path:          /invalid
Document Length:        64 bytes

Concurrency Level:      1000
Time taken for tests:   0.287 seconds
Complete requests:      2000
Failed requests:        977
   (Connect: 0, Receive: 0, Length: 977, Exceptions: 0)
Total transferred:      451402 bytes
Total body sent:        764000
HTML transferred:       153402 bytes
Requests per second:    6964.64 [#/sec] (mean)
Time per request:       143.583 [ms] (mean)
Time per request:       0.144 [ms] (mean, across all concurrent requests)
Transfer rate:          1535.08 [Kbytes/sec] received
                        2598.14 kb/s sent
                        4133.22 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   18  17.9     24      47
Processing:    12   38  26.3     28     103
Waiting:       12   37  26.5     23     103
Total:         12   56  37.0     72     112

Percentage of the requests served within a certain time (ms)
  50%     72
  66%     81
  75%     90
  80%     94
  90%    102
  95%    106
  98%    109
  99%    111
 100%    112 (longest request)
```



nginx_conf : worker_processes 10


```
ab -p json.txt -T application/json -c 100 -n 2000 http://127.0.0.1:6699/invalid
```


```
Document Path:          /invalid
Document Length:        64 bytes

Concurrency Level:      100
Time taken for tests:   0.287 seconds
Complete requests:      2000
Failed requests:        124
   (Connect: 0, Receive: 0, Length: 124, Exceptions: 0)
Total transferred:      429224 bytes
Total body sent:        764000
HTML transferred:       131224 bytes
Requests per second:    6957.15 [#/sec] (mean)
Time per request:       14.374 [ms] (mean)
Time per request:       0.144 [ms] (mean, across all concurrent requests)
Transfer rate:          1458.09 [Kbytes/sec] received
                        2595.34 kb/s sent
                        4053.44 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   1.6      0       8
Processing:     3   13   4.0     13      40
Waiting:        3   13   3.9     12      39
Total:          7   14   3.4     13      40

Percentage of the requests served within a certain time (ms)
  50%     13
  66%     15
  75%     16
  80%     16
  90%     17
  95%     20
  98%     23
  99%     25
 100%     40 (longest request)
```

#### 传递异常的json:
worker_processes 10:
```
ab -p json_null.txt -T application/json -c 1000 -n 9000 http://127.0.0.1:6699/invalid
```

```
Document Path:          /invalid
Document Length:        17 bytes

Concurrency Level:      1000
Time taken for tests:   1.023 seconds
Complete requests:      9000
Failed requests:        0
Total transferred:      1494000 bytes
Total body sent:        1395000
HTML transferred:       153000 bytes
Requests per second:    8793.38 [#/sec] (mean)
Time per request:       113.722 [ms] (mean)
Time per request:       0.114 [ms] (mean, across all concurrent requests)
Transfer rate:          1425.49 [Kbytes/sec] received
                        1331.03 kb/s sent
                        2756.52 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        6   22   7.1     22      48
Processing:     7   40  19.9     37     138
Waiting:        5   33  18.7     30     130
Total:         20   62  22.0     60     155

Percentage of the requests served within a certain time (ms)
  50%     60
  66%     64
  75%     69
  80%     73
  90%     79
  95%    108
  98%    143
  99%    146
```


### 1.5 invalid 模块debu.log显示的内容
```
2017/09/22 11:27:49 [error] 13088#13088: *9186 [lua] resty_mysql.lua:44: conn(): failed to connect: failed to receive packet header: timeout: nil nil, context: ngx.timer, client: 127.0.0.1, server: 0.0.0.0:6699

```

```
2017/09/22 10:28:05 [error] 32158#32158: *1136 [lua] invalid.lua:242: get_message(): failed to create the timer: too many pending timers, client: 127.0.0.1, server: , request: "POST /invalid HTTP/1.0", subrequest: "/invalid_instance", host: "127.0.0.1:6699"

```


### 1.5 结论
worker_processes 调大后能够使用更多的定时器、更多的资源


## wrk工具
[原文链接](http://www.restran.net/2016/09/27/wrk-http-benchmark/)

### install wrk

CentOS / RedHat / Fedora
```
sudo yum groupinstall 'Development Tools'
sudo yum install openssl-devel
sudo yum install git
git clone https://github.com/wg/wrk.git wrk
cd wrk
make
# move the executable to somewhere in your PATH
sudo cp wrk /somewhere/in/your/PATH
```


### 使用方法

```
wrk -t4 -c1000 -d30s -T30s --latency http://www.douban.com
```

```
Running 30s test @ http://www.douban.com
  4 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    18.50s   854.51ms  20.38s    66.21%
    Req/Sec    18.88     20.80    67.00     80.00%
  Latency Distribution
     50%   18.52s
     75%   19.06s
     90%   19.79s
     99%   20.35s
  367 requests in 30.09s, 167.73KB read
  Socket errors: connect 0, read 380, write 0, timeout 0
Requests/sec:     12.20
Transfer/sec:      5.57KB
```

主要关注的几个数据是

```
Socket errors socket 错误的数量
Requests/sec 每秒请求数量，也就是并发能力
Latency 延迟情况及其分布
```

### wrk 支持使用 lua 来写脚本
wrk 的代码中 scripts 文件夹中就给出了不少例子，例如 post.lua，可以根据需要来修改。

```
-- example HTTP POST script which demonstrates setting the
-- HTTP method, body, and adding a header
wrk.method = "POST"
wrk.body   = "foo=bar&baz=quux"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
```
可以这样来调用 script


```
wrk -t4 -c100 -d30s -T30s --script=post.lua --latency http://www.douban.com
```
### 用 lua 脚本测试复杂场景

wrk 提供了几个 hook 函数，可以用 lua 来编写一些复杂场景下的测试：

#### setup

这个函数在目标 IP 地址已经解析完，并且所有 thread 已经生成，但是还没有开始时被调用，每个线程执行一次这个函数。可以通过 thread:get(name)， thread:set(name, value) 设置线程级别的变量。

#### init

每次请求发送之前被调用。可以接受 wrk 命令行的额外参数，通过 – 指定。

#### delay

这个函数返回一个数值，在这次请求执行完以后延迟多长时间执行下一个请求，可以对应 thinking time 的场景。

#### request

通过这个函数可以每次请求之前修改本次请求的属性，返回一个字符串，这个函数要慎用， 会影响测试端性能。

#### response

每次请求返回以后被调用，可以根据响应内容做特殊处理，比如遇到特殊响应停止执行测试，或输出到控制台等等。

可以查看 scripts 文件夹下的例子，例如 delay.lua，实现的是每个请求前会有随机的延迟。

```
-- example script that demonstrates adding a random
-- 10-50ms delay before each request
function delay()
   return math.random(10, 50)
end
```
### 使用wrk测试

post.lua
```
-- example HTTP POST script which demonstrates setting the
-- HTTP method, body, and adding a header

wrk.method = "POST"
wrk.body   = "{\"header\": {\"send_tm\": \"20170915\",\"type\": 0},\"invalid_data\": {\"name\": \"fsd\",\"expire_tm\": 3,\"cond\": {\"data\": {\"filed\": \"tt_filed\",\"start_tm\": \"%lastmonth_3\",\"end_tm\": \"%lastmonth_1\"},\"tname\": \"test\"},\"conf\": {\"driver\": \"mysql\",\"db\": {}}}}"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
```



command
```
./wrk -t12 -c100 -d30s -T30s --script=post.lua http://10.1.113.178:6699/invalid

```
show

```
Running 30s test @ http://10.1.113.178:6699/invalid
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    83.59ms   59.66ms 541.63ms   63.26%
    Req/Sec   607.44    238.32     1.71k    74.95%
  210512 requests in 30.11s, 52.65MB read
  Socket errors: connect 0, read 2944, write 0, timeout 0
Requests/sec:   6991.91
Transfer/sec:      1.75MB
```


#### wrk命令说明
```
Usage: wrk <options> <url>                            
  Options:                                            
    -c, --connections <N>  Connections to keep open   
    -d, --duration    <T>  Duration of test           
    -t, --threads     <N>  Number of threads to use   

    -s, --script      <S>  Load Lua script file       
    -H, --header      <H>  Add header to request      
        --latency          Print latency statistics   
        --timeout     <T>  Socket/request timeout     
    -v, --version          Print version details
    -T  timeout
```












[上一级](base.md)
[上一篇](google_search_tips.md)
