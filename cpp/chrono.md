# Clock 和 Timer


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Clock 和 Timer](#clock-和-timer)
	* [chrono 程序库概览](#chrono-程序库概览)
		* [duration](#duration)
		* [clock（时钟） 和 Timepoint（时间点）](#clock时钟-和-timepoint时间点)
			* [clock](#clock)
			* [timepoint](#timepoint)
		* [C 和 POSIX 提供的 Date/Time 函数](#c-和-posix-提供的-datetime-函数)
			* [timepoint 与 日历时间的转换](#timepoint-与-日历时间的转换)
		* [以计时器停滞线程](#以计时器停滞线程)

<!-- /code_chunk_output -->


编程语言最该拥有、最不可或缺的程序库，应该是用来处理日期和时间的程序库。

在过去，C和POSIX提供的系统时间接口，允许从秒转换至毫秒，再至微秒，最终至纳秒，问题是每次转换就需要一个新的接口。基于这个原因，C++11 开始提供一个精度中立的程序库，它常被称为chrono库，因为它的特性被定义于 \<chrono\>。
此外，c++ 标准库也提供一个基本的C和POSIX接口，用来处理日历时间。

## chrono 程序库概览

c++ 标准库把 duration(时间段)和 timepoint(时间点) 从特定的 clock(时钟)区分开来：
* duration:指的是在某时间单位上的一个明确的 tick（片刻数)。例如：“3分钟”时间段意指“3个一分钟”。
* timepoint:是一个duration 和一个 epoch(起始点) 的组合。

### duration

[class duration](http://en.cppreference.com/w/cpp/chrono/duration) 是一个数值(表现 tick 个数)和一个分数（表现时间单位，以秒计） 的组合。其中的分数 由 [class ratio](http://www.cplusplus.com/reference/ratio/ratio/) 描述。
例如：
```c++
std::chrono::duration<int> twentySeconds(20);
std::chrono::duration<double,std::ratio<60>> halfAMinute(0.5);
std::chrono::duration<long,std::ratio<1,1000>> oneMillisecond(1);
```

为了提供更多方便性，C++ 标准库提供了以下各种类型定义：

| Type    | Definition    |
| :------------- | :------------- |
|std::chrono::nanoseconds	|duration</\*signed integer type of at least 64 bits\*/, std::nano>
|std::chrono::microseconds	|duration</\*signed integer type of at least 55 bits\*/, std::micro>
|std::chrono::milliseconds	|duration</\*signed integer type of at least 45 bits\*/, std::milli>
|std::chrono::seconds	|duration</\*signed integer type of at least 35 bits\*/>
|std::chrono::minutes	|duration</\*signed integer type of at least 29 bits\*/, std::ratio<60>>
|std::chrono::hours	|duration</\*signed integer type of at least 23 bits\*/, std::ratio<3600>>

有了它们，我们可以轻松的指定时间段：
```c++
std::chrono::seconds twentySeconds(20);
```

duration 也支持算术运算（运算所涉及的两个duration 的单位类型可以不同）

你也可以将 duration 转换为不同单位，只要彼此之间存在隐式转换就行。

```c++
std::chrono::seconds twentySeconds(20);
std::chrono::hours aDay(24);

std::chrono::milliseconds ms;
ms+=twentySeconds+aDay;
--ms;
ms*=2;
std::cout << ms.count()<<" ms" << '\n';
// count() 产出当前的 tick 数量
std::cout << std::chrono::nanoseconds(ms).count()<<" ns" << '\n';
```

这些转换导致以下输出：
```terminal
172839998 ms
172839998000000 ns
```

你可以隐式转换至一个“教精准的单位类型”可行，转换至“教粗糙的单位类型”就不可行了。但你可以使用 duration_cast 强制转换
```c++
std::chrono::seconds sec(55);
std::chrono::minutes m1=sec; //error，不允许这样的隐式转换
std::chrono::minutes m2=std::chrono::duration_cast<std::chrono::minutes>(sec);
//ok,强制转换为“教粗糙的单位类型”，实际数值会被截掉而非四舍五入，结果：m2.count()为0
```

转换“浮点数 tick”类型的 duration,令它变成 “整数 tick”类型，也必须给它一个明显的类型转换动作：
```c++
std::chrono::duration<double,std::ratio<60>> halfMin(0.5);
std::chrono::seconds s1=halfMin; //error
auto s2=std:chrono::duration_cast<std::chrono::seconds>(halfMin); //ok
```

我们可以把 duration 切割为不同的单元。
```c++
using namespace std;
using namespace std::chrono;

milliseconds ms(7255042);

//split
hours hh=duration_cast<hours>(ms);
minutes mm=duration_cast<minutes>(ms % chrono::hours(1));
seconds ss=duration_cast<seconds>(ms % chrono::minutes(1));
milliseconds msec=duration_cast<milliseconds>(ms % chrono::seconds(1));
```

### clock（时钟） 和 Timepoint（时间点）

Timepoint 和 Clock 的关系有点微妙：
* Clock 定义出一个 epoch （起始点） 和一个tick周期。例如某个 clock 也许定义 tick 周期为毫秒，起始点是 UNIX epoch （1970.01.01）, 或定义 tick 周期为纳秒，起始点是程序开始时间。此外，clock 还提供一个类型给 “与此 clock 关联”的任何 timepoint 使用
* Timepoint 表现出某个特定时间点，关联至某个 clock 的某个正值或负值 duration。因此，如果 duration 是 10 天而其所关联的 clock epoch 是“1970.01.01”，那么这个 timepoint 表现的就是 1970.01.11.

Timepoint 提供的能力包括：产出 epoch、产出“与其 clock 相应” 的所有timepoint 中的最小值和最大值，以及 timepoint的各种算术运算。

#### clock

[c++ concepts:clock](http://en.cppreference.com/w/cpp/concept/Clock) 提供的类型定义和 static 成员：

| 操作 | 效果     |
| :------------- | :------------- |
clock::duration | 获得 clock 的 duration 类型
clock::rep  | 获得 tick 类型（等价于 clock::duration::rep）
clock::period  | 获得 单位类型（等价于 clock::duration::period）
clock::time_point |  获得 clock 的 timepoint 类型
clock::is_steady  |获得 true - 如果 clock 是 steady
clock::now() |获得一个用来表现目前时间的 time_point

c++ 标准库提供了三个 clock，每一个都具备上述接口：
1. [class system_clock](http://en.cppreference.com/w/cpp/chrono/system_clock),它所表现的 timepoint将关联至现行系统的即时时钟（real-time clock）。这个 clock 提供了便捷函数 to_time_t(),from_time_t()方便与 “C 的系统时间类型” time_t 之间转换。
2. [class steady_clock](http://www.cplusplus.com/reference/chrono/steady_clock),它保证绝不会被调整。被设计成用于比较程序中两个时间点或计算其差距。
3. [class high_resolution_clock](http://en.cppreference.com/w/cpp/chrono/high_resolution_clock),它表现的是当前系统中带有最短 tick 周期的 clock。

如果想要比较程序中两个时间点或计算差距，steady_clock 将扮演重要角色。例如：
```c++
auto system_start=chrono::system_clock::now();
if (chrono::system_clock::now() > system_start + minutes(1))
```
这也许行不通，因为如果clock在此期间被调整，上述比较结果有可能是 false，即使这个程序的执行超过一分钟。类似的
```c++
auto diff=chrono::system_clock::now() - system_start;
auto sec=chrono::duration_cast<chrono::seconds> (diff);
std::cout << "this program runs: "<<s.count<<"	seconds" << '\n';
```
有可能因为此期间clock被调整而打印出一个负值 duration 。基于相同的理由，使用 timer 搭配 steady_clock 以外的clock，有可能一旦 system_clock 被调整，它们的 duration 也随之改变。

#### timepoint

[class time_point](http://en.cppreference.com/w/cpp/chrono/time_point)

下面四个特定的 timepoint 扮演了特殊角色：
1. epoch，由任何 clock 的 time_point 的 default 构造函数产出
2. current time,由任何 clock 的 static 成员函数 now() 产出
3. minimum timepoint,由任何 clock 的 time_point 的 static 成员函数 min() 产出
4. maximum timepoint,由任何 clock 的 time_point 的 static 成员函数 max() 产出

```c++
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

string asString(const chrono::system_clock::time_point &tp) {
  time_t t = chrono::system_clock::to_time_t(tp);
  string ts = ctime(&t);    // ctime() 会考虑时区
  ts.resize(ts.size() - 1); // skap trailing newline
  return ts;
}

int main(int argc, char const *argv[]) {
  // print the epoch of this system clock
  chrono::system_clock::time_point
      tp; // 等同于 chrono::time_point<std::chrono::system_clock>
  std::cout << "epoch: " << asString(tp) << '\n';

  // print current time
  tp = chrono::system_clock::now();
  std::cout << "epoch: " << asString(tp) << '\n';

  // print minimum time of this system clock
  tp = chrono::system_clock::time_point::min();
  std::cout << "epoch: " << asString(tp) << '\n';

  // print maximum time of this system clock
  tp = chrono::system_clock::time_point::max();
  std::cout << "epoch: " << asString(tp) << '\n';

  return 0;
}
```

Run:
```terminal
epoch: Thu Jan  1 08:00:00 1970
epoch: Fri Jan 12 16:36:25 2018
epoch: Tue Sep 21 08:18:27 1677
epoch: Sat Apr 12 07:47:16 2262
```

虽然 time_point 的接口用了 class ratio ，这确保了 duration 单元的溢出会导致编译器差错，但 duration 的溢出还是可能发生。

```c++
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

string asString(const chrono::system_clock::time_point &tp) {
  time_t t = chrono::system_clock::to_time_t(tp);
  string ts = ctime(&t);
  ts.resize(ts.size() - 1); // skip trailing newline
  return ts;
}

int main(int argc, char const *argv[]) {
  // define type for durations that represent day(s)
  typedef chrono::duration<int, ratio<3600 * 24>> Days;

  // process the epoch of this system clock
  chrono::time_point<chrono::system_clock> tp;
  std::cout << "epoch: " << asString(tp) << '\n';

  // add one day,23 hours,and 55 minutes
  tp += Days(1) + chrono::hours(23) + chrono::minutes(55);
  std::cout << "later:  " << asString(tp) << '\n';

  // process difference from epoch in minutes and days
  auto diff = tp - chrono::system_clock::time_point();
  std::cout << "diff: " << chrono::duration_cast<chrono::minutes>(diff).count()
            << "  minutes(s)" << '\n';
  Days days = chrono::duration_cast<Days>(diff);
  std::cout << "diff: " << days.count() << "  day(s)" << '\n';

  // subtract one year (hoping it is vaild and not a leap year)
  tp -= chrono::hours(24 * 365);
  std::cout << "-1 year:  " << asString(tp) << '\n';

  // subtract 50 year(hoping it is vaild and ignoring leap years)
  tp -= chrono::duration<int, ratio<3600 * 24 * 365>>(50);
  std::cout << "-50 years:  " << asString(tp) << '\n';

  // subtract 260 year(hoping it is vaild and ignoring leap years)
  tp -= chrono::duration<int, ratio<3600 * 24 * 365>>(260);
  //这些操作并不校验任何组合运算是否造成溢出
  std::cout << "-260 years:  " << asString(tp) << '\n';
  return 0;
}
```

Run:
```terminal
epoch: Thu Jan  1 08:00:00 1970
later:  Sat Jan  3 07:55:00 1970
diff: 2875  minutes(s)
diff: 1  day(s)
-1 year:  Fri Jan  3 07:55:00 1969
-50 years:  Thu Jan 16 07:55:00 1919
-260 years:  Sun Oct  8 07:29:33 2243
```

注意：最后的 - 260 years 得到的结果明显不对。（这些操作并不校验任何组合运算是否会造成溢出）
这也说明了 chrono 是一个 duration/timepoint 程序库，不是个 date/time 程序库。你可以计算 duration 和 timepoint 但仍然必须把 epoch 、 最小和最大的 timepoint、闰年和润秒纳入考虑。


### C 和 POSIX 提供的 Date/Time 函数

c++ 标准库也提供 C 和 POSIX 所提供的 “处理 date 和 time”接口，在<ctime> 的 namespace std 内。

| 识别名称 | 意义     |
| :------------- | :------------- |
clock_t | 数值类型，用来表现 elapsed CPU time，由 clock() 返回
time_t | 数值类型，用来表现 timepoint
struct tm | 被解开 (broken down) 之 日历时间的类型
clock() | 获得 elapsed CPU time ,单位 是(1/CLOCKS_PER_SEC) 秒
time() | 获得当前时间，是个数值
difftime() | 获得两个 time_t 之间的差额，是个 double 值，单位为秒
localtime() | 转换 time_t 成为一个 struct tm,考虑时区
gmtime() | 转换 time_t 成为一个 struct tm,不考虑时区
asctime()| 转换 struct tm 成为一个标准日历时间字符串
strftime() | 转换 struct tm 成为一个用户自定义的日历时间字符串
ctime() | 转换 time_t 成为一个标准日历时间字符串，并且考虑时区,相当于asctime(localtime(t))
mktime() | 转换 struct tm 成为一个 time_t 并查询其为“星期中的哪一天”(weekday)和一年中的第几天(day of year)

#### timepoint 与 日历时间的转换

转换 timepoint 成为日历时间 （一个 string）的便捷函数：

timepoint.hpp:
```c++
#include <chrono>
#include <ctime>
#include <string>

inline std::string asString(const std::chrono::system_clock::time_point &tp) {
  std::time_t t = std::chrono::system_clock::to_time_t(tp);
  std::string ts = ctime(&t);
  ts.resize(ts.size() - 1); // skip trailing newline
  return ts;
}

inline std::chrono::system_clock::time_point
makeTimePoint(int year, int mon, int day, int hour, int min, int sec = 0) {
  struct std::tm t;
  t.tm_sec = sec;          // second of minutes(0..59)
  t.tm_min = min;          // minute of hour (0..59)
  t.tm_hour = hour;        // hour of day(0..23)
  t.tm_mday = day;         // day of month(0..31)
  t.tm_mon = mon - 1;      // month of year (0..11)
  t.tm_year = year - 1900; // year since 1900
  t.tm_isdst = -1;         // determine wether daylight saving time
  std::time_t tt = std::mktime(&t);
  if (tt == -1) {
    throw "no vaild system time";
  }
  return std::chrono::system_clock::from_time_t(tt);
}
```

timepoint1.cpp:
```c++
#include "timepoint.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char const *argv[]) {
  auto tp1 = makeTimePoint(2010, 01, 01, 00, 00);
  std::cout << asString(tp1) << '\n';

  auto tp2 = makeTimePoint(2011, 05, 23, 13, 44);
  std::cout << asString(tp2) << '\n';
  return 0;
}
```

Run:
```terminal
Fri Jan  1 00:00:00 2010
Mon May 23 13:44:00 2011
```

### 以计时器停滞线程

duration 和 timepoint 可用于线程或程序的停滞。停滞可以是无条件的，也可以指定最大时间段，或等待一个lock或条件成立，或等待另一线程结束：
* sleep_for() 和 sleep_until(),由 this_thread 提供用以停滞线程
* try_lock_for() 和 try_lock_until() ,用来等待一个mutex时指定最大时间段
* wait_for() 和 wait_until(),用来等待某条件成立或等待一个future时指定最大时间段

用 ...\_for() 结束的停滞函数都会用到 duration， ...\_until() 结束的函数都会用到 timepoint


[上一级](base.md)
[上一篇](STL.md)
[下一篇](compareDoubleFloat.md)
