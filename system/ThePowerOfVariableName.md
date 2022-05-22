<!-- 变量名的力量 -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [最重要的命名注意事项](#最重要的命名注意事项)
- [以问题为导向](#以问题为导向)
- [最适当的名字长度](#最适当的名字长度)
- [作用域对变量名的影响](#作用域对变量名的影响)
- [变量名中的计算值限定词](#变量名中的计算值限定词)
- [变量中的常用对仗词](#变量中的常用对仗词)
- [为特定类型的数据命名](#为特定类型的数据命名)
  - [为循环下标命名](#为循环下标命名)
  - [为状态变量命名](#为状态变量命名)
  - [为临时变量命名](#为临时变量命名)
  - [为布尔变量命名](#为布尔变量命名)
  - [为枚举类型命名](#为枚举类型命名)
  - [为常量命名](#为常量命名)
- [与语言相关的命名规则的指导原则](#与语言相关的命名规则的指导原则)
  - [C 的命名规则](#c-的命名规则)
  - [C++ 的命名规则](#c-的命名规则-1)
  - [Java 的规则](#java-的规则)
- [混合语言编程的注意事项](#混合语言编程的注意事项)
- [命名规则示例](#命名规则示例)
- [链接](#链接)

<!-- /code_chunk_output -->

## 最重要的命名注意事项
为变量命名是最重要的考虑事项是，该名字要完全、准确地描述出该变量所代表的事物。通常，对变量的描述就是最佳的变量名。这种名字很容易阅读，因为其中并不包含晦涩的缩写，同时也没有歧义。因为它是对该事物的完整描述，因此不会和其他事物混淆。另外，由于这一名字与所表达的概念相似，因此也很容易记忆。

对于一个表示美国奥林匹克代表团成员数量的变量，你可能会把它命名为 numberOfPeopleOnTheUSOlympicTeam.
表示运动场中座椅数量的变量可能命名为 numberOfSeatsInTheStadium. 表示某国代表团在现代奥运会上获得的最高分数的变量可能会命名为 maximumNumberOfPointsInModernOlympics .

请留意上述这些命名所共有的两个特性：
1. 很容易理解
2. 有些名字太长了-长的很不实用

更多变量名的例子，其中有好的也有差的

| 变量用途           | 好名字，好描述                       | 坏名字，差描述                   |
|--------------------|--------------------------------------|----------------------------------|
| 到期的支票累计额   | runningTotal,checkTotal              | written,ct,checks,CHKTTL,x,x1,x2 |
| 高速列车的运行速度 | velocity,trainVelocity,velocityInMph | velt,v,tv,x,x1,x2,train          |
| 当前日期           | currentDate,todaysData               | cd,current,c,x,x1,x2,date        |
| 每页的行数         | linesPerPage                         | lpp,lines,l,x,x1,x2              |

## 以问题为导向
一个好记的名字反映的通常都是问题，而不是解决方案。一个好名字通常表达的是 “什么” （what），而不是 “如何”（how）。一般而言，如果一个名字反映了计算的某些方面而不是问题本身，那么它反映的就是 “how” 而非 “what”了。请避免取这样的名字，而应该在名字中反映出问题本身。

一条员工数据记录可以称作 inputRec 或者 employeeData 。inputRec 是一个反映输入、记录这些计算概念的计算机术语。 employeeData 则直指问题领域，与计算的世界无关。相比较的话，使用 employeeData 命名比 inputRec 更好。

## 最适当的名字长度
变量名的最佳长度似乎应该介于 x 和 maximumNumberOfPointsInModernOlympics 之间。
* 太短的名字无法传达足够的信息
* 太长的名字很难写，同时也会使程序的视觉结构变得模糊不清

Gorla、Benander 发现，当变量名的平均长度在 10 到 16 个字符的时候，调试程序所需花费的气力是最小的 （1990）。平均名字长度在 8 到 20 个字符的程序也几乎同样容易调试。这项原则并不意味着你应该尽量把变量名控制在这个范围内。它强调的是，如果你查看自己写的代码时发现了很多更短的名字，那么你需要认真的检查，确保这些名字的含义足够清晰。

你可能已经通过 Goldilocks-and-the-Tree-Bears （[金发姑娘与三只小熊的经典通话](https://zh.m.wikipedia.org/zh-hans/%E9%87%91%E9%AB%AE%E5%A7%91%E5%A8%98%E5%8E%9F%E5%89%87)，寓意权衡比较）的方法理解了如何为变量命令，如下表所示：

| 长度      |  示例      |
| :------------- | :------------- |
| 太长 | numberOfPeopleOnTheUSOlympicTeam numberOfSeatsInTheStadium  maximumNumberOfPointsInModernOlympics |
| 太短 | n,np,ntm,ms,nsisd,m,mp,max,points                                                              |
| 正好 | numTeamMembers,teamMemberCount, numSeatsInStadium,seatCount, teamPointsMax,pointsRecord           |

## 作用域对变量名的影响
短的变量名总是不好吗？不，不总是这样。当你把一个变量名取得非常短的时候，如 i ，这一长度本身就对该变量做出了一些说明--也就是说，该变量代表的是一个临时的数据，他的作用域非常有限。

较长的名字适用于很少用到的变量或者全局变量，而较短的名字则适用于局部变量或者循环变量。不过，短的变量名常常会带来一些麻烦，因此，作为一项防御式编程策略，一些细心的程序员会避免使用短的变量名。

对位于全局命名空间中的名字加以限定词。在 C++ 中，你可以使用 namespace 关键词来划分全局命名空间。

## 变量名中的计算值限定词
很多程序都有表示计算结果的变量：总额、平均值、最大值，等等。如果你要用类似于 Total、Sum、Average、Max、Min、Record、String、Pointer 这样的限定词来修改某个名字，那么请记住把限定词加到名字的最后。

这种方法具有很多优点：
* 变量名中最重要的那部分，即为这一变量赋予主要含义的部分应当位于最前面，这样，这一部分就可以显得最为突出，并会被首先阅读到
* 采纳了这一规则，你将避免由于同时在程序中使用 totalRevenue 和 revenueTotal 而产生的歧义。

把计算的量放到名字最后这条规则也有例外，那就是 Num 限定词的位置已经是约定俗成的。
* Num 放在变量名的开始位置代表一个总数： numCustomers 表示的是员工的总数
* Num 放在变量名的结束位置代表的是一个下标：customerNum 表示当前员工的序号

由于这样使用 Num 常常会带来麻烦，因此可能最好的办法是避开这些问题，用 Count 或者 Total 来代表员工的总数，用 Index 来指代某个特定的员工。这样 customerCount 就代表员工的总数，customerIndex 代表某个特定的员工。

## 变量中的常用对仗词
* begin/end
* first/last
* locked/unlocked
* min/max
* next/previous
* old/new
* opened/closed
* visible/invisible
* source/target
* source/destination
* up/down

## 为特定类型的数据命名
在为数据命名的时候，除了通常的考虑事项之外，为一些特定类型数据的命名还要求做出一些特殊的考虑。

### 为循环下标命名
循环是一种极为常见的计算机编程特征，为循环中的变量进行命名的原则也由此应运而生。 i,j,k 这些名字都是约定俗成的：

简单的循环变量名：
```c++
for (size_t i = firstItem; i < lastItem; i++) {
	data[i]=0;
}
```

嵌套循环中的好循环变量名：
```c++
for (size_t teamIndex = 0; teamIndex < teamCount; teamIndex++) {
	for (size_t eventIndex = 0; eventIndex < eventCount[teamIndex]; eventIndex++) {
		sore[teamIndex][eventIndex]=0;
	}
}
```

谨慎地为循环下标变量命名可以避免下标串话（index cross-talk） 的常见问题：想用 j 的时候写了 i，想用 i 的时候时候却写了 j。同时，这也使得数据访问变得更加清晰： score[teamIndex][eventIndex] 要比 score[i][j] 给出的信息更多。

如果你一定要用 i,j,k,那么不要把他们用于简单循环的循环下标之外的任何场合-这种传统已经太深入人心了，一旦违背该原则，将这些变量用于其他用途就可能造成误解。要想避免出现这些问题，最简单的方法就是想出一个比 i、j和 k 更具描述性的名字来。

### 为状态变量命名

状态变量用于描述你的程序的状态。下面给出它的命名规则。

为状态u变量取一个 flag 更好的名字。最好把标记 (flag) 看做状态变量。标记的名字中不应该含有 flag,因为你从中丝毫看不出该标记是做什么的。为了清楚起见，标记应该用枚举类型、具名常量，或用作具名常量的全局变量来对其赋值，而且其值应该与上面这些量做比较。下面例子中标记俄命名都很差：
```c++
if (flag) ...
if (statusFlag & 0x0F) ...
if (printFlag == 16) ...
if (computeFlag == 0) ...
flag=0x1;
statusFlag=0x80;
printFlag =16;
computeFlag=0;
```

下面是作用相同但更为清晰的代码：
```c++
if (dataReady) ...
if (characterType & PRINTABLE_CHAR) ...
if (reportType==ReportType_Annual) ...
if (recalcNeeded == false ) ...

dataReady=true;
characterType=CONTROL_CHARACTER;
reportType=ReportType_Annual;
recalcNeeded=false;
```

下面例子展示了如何使用具名常量和枚举类型来组织例子中的数值：
```C++
//values for CharacterType
const int LETTER=0x01;
const int DIGIT=0x02;
const int PUNCTUATION= 0x04;
cosnt int LINE_DRAW= 0x08;

const int PRINTABLE_CHAR=(LETTER | DIGIT | PUNCTUATION | LINE_DRAW);

CONST INT CONTROL_CHARACTER=0X80；

// values for ReportType
enum ReportType{
	ReportType_Daily,
	ReportType_Monthly,
	ReportType_Quarterly,
	ReportType_Annual,
	ReportType_All
};
```

 如果你发现自己需要猜测某段代码的含义的时候，就该考虑为变量重新命名。猜测谋杀案中谁是神秘凶手是可行的，但你没有必要去猜测代码。你应该能直接读懂它们。

### 为临时变量命名

警惕 “临时” 变量，考虑如下的两个例子：

```c++
//不提供信息的 “临时” 变量名
temp=sqrt(b^2 -4*a*c);
root[0] =(-b + temp)/ (2 * a);
root[1] =(-b - temp)/(2 * a);
```

```c++
//用真正的变量替代 “临时” 变量
discriminant=sqrt(b^2 -4*a*c);
root[0] =(-b + discriminant)/ (2 * a);
root[1] =(-b - discriminant)/(2 * a);
```
就本质而言，这段代码与上面一段是完全相同的，但是它却通过使用了准确而且具有描述性的变量名 (discriminant,判别式) 而得到了改善。

### 为布尔变量命名

下面是为布尔变量命名是要遵循的几条原则。

* 谨记典型的布尔变量名：
	* done 用 done 表示某件事情已经完成。在事情完成之前把 done 设为 false，在事情完成之后把它设为 true
	* error 用 error 来表示有错误发生。在错误发生之前把变量设为 false，在错误已经发生时把它设为 true
	* found 用 found 来表明某个值已经找到了。在还没有找到该值的时候把 found 设为 false，一旦找到该值就把 found 设为 true。
	* success 或 ok 用 success 或 ok 来表明一项操作是否成功。在操作失败的时候把变量设为 false，在操作成功的时候把其设为 true。如果可以，请用一个更具体的名字代替 success ,以便更具体地描述成功的含义。如果完成处理就表示这个程序执行成功，那么或许你应该用 processingComplete 来取而代之。如果找到某个值就是程序执行成功，那么你也许应该换用 found。

* 给布尔变量赋予隐含 “真/假” 含义的名字,避免使用 "status" 这样的名字
* 使用肯定的布尔变量名
否定的名字如 notFound ， notDone , notSuccessful 等较难阅读，特别是如果他们被求反：
```hightlight
if not notFound
```
### 为枚举类型命名
在使用枚举类型的时候，可以通过使用组前缀，如 Color_, Planet_ 或者 Month_ 来明确表示该类型的成员都同属一个组。示例：
```c++
enum Color {
	Color_Red,
	Color_Green,
	Color_Blue
  };

enum Planet {  
	Planet_Earth,
	Planet_Mars,
	Planet_Venus
};

enum Month {  
	Month_January,
	Month_February,
	...
	Month_December
};
```

### 为常量命名
在具名常量时，应该根据该常量所表示的含义，而不是该常量所具有的数值为该抽象事物命名。

## 与语言相关的命名规则的指导原则

### C 的命名规则
有很多命名规则特别适用于 C 语言。
* c 和 ch 是字符变量
* i 和 j 是整数下标
* n 表示某物的数量
* p 是指针
* s 是字符串
* 预处理宏全部大写(ALL_CAPS)。这通常也包括 typedef.
* 变量名和子程序名全部小写(all_lowercase)
* 下划线(\_) 用作分隔符： letters_in_lowercase 要比 lettersinlowercase 更具可读性。

这些都是属于一般性的、 UNIX 风格或者 Linux 风格的 C 编程规则， C 编程规则在不同的环境下也会有所差异。

### C++ 的命名规则
以下是围绕着 C++ 编程形成的命名规则。
* i 和 j 是整数下标
* p 是指针
* 常量、typedef 和预处理宏全部大写 (ALL_CAPS)
* 类和其他类型的名字混合大小写（MixedUpperAndLowerCase()）
* 变量名和函数名中的第一个单词小写，后续每个单词的首字母大写-例如， variableOrRoutineName。
* 不要把下划线用做名字中的分隔符，除非用于全部大写的名字以及特定的前缀中（如用于标识全局变量的前缀）

与 C 编程相比，上述规则还远没有形成标准，并且不同的环境也会形成不同的具体规则。

### Java 的规则
与 C 和 C++ 的不同， Java 语言的风格约定从一开始就创建好了。
* i 和 j 是整数下标
* 常量全部大写(ALL_CAPS) 并使用下划线分隔。
* 类名和接口名中每一个单词的首字母均大写，包括第一个单词- 例如，ClassOrInterfaceName.
* 变量名和方法名中第一个单词的首字母小写，后续单词的首字母大写 - 例如，variableOrRoutineName.
* 除用于全部大写的名字之外，不使用下画线作为名字中的分隔符。
* 访问器子程序使用 get 和 set 方法。

## 混合语言编程的注意事项

在该书中，变量名均以小写开头，这符合 Java 的编程实践传统以及部分但并非全部的 C++ 传统。该书中把所有子程序名的首字母大写，这遵循了 C++ 规则。在 Java 中所有的方法名都是小写字母开始的，但是该书对所有语言的子程序名的首字母都大写，从而提高了整体可读性。

## 命名规则示例

变量名包含了以下三类信息：
* 变量的内容（它代表了什么）
* 数据的种类（具名常量、简单变量、用户自定义类型或者类）
* 变量的作用域（私有的、类的、包的、或者全局的作用域）
根据上述的指导原则，下表给出了一些语言的命名规则。这些特殊规则并非是强制性的，但是他们能帮你了解一份非正式的命名规则应包含那些内容。

C++ 和 Java 的命名规则示例

| 实体                | 描述                                                                                             |
|---------------------|--------------------------------------------------------------------------------------------------|
| ClassName           | 类名混合使用大小写，首字母大写                                                                   |
| TypeName            | 类型定义，包括枚举类型和typedef，混合使用大小写，首字母大写                                      |
| EnumeratedTypes     | 除遵循上述规则之外，枚举类型总以复数形式表示                                                     |
| localVariable       | 局部变量混合使用大小写，首字母小写。其名字应该与底层数据类型无关，而且应该反映该变量所代表的事物 |
| routineParameter    | 子程序参数的格式与局部变量相同                                                                   |
| RoutineName()       | 子程序名混合使用大小写                                                                           |
| m_classVariable     | 对类的多个子程序可见的成员变量名m_前缀                                                           |
| g_GlobalVariable    | 全局变量名用 g_ 前缀                                                                             |
| CONSTANT            | 具名常量全部大写                                                                                 |
| MACRO               | 宏全部大写                                                                                       |
| Base_EnumeratedType | 枚举类型名用能够反映其基础类型的、单数形式的前缀- 例如，Color_Red,Color_Blue                     |




## 链接
* 《代码大全 2rd》 第 11 章

---
- [上一级](README.md)
- 上一篇 -> [极客时间 - 软件工程之美（宝玉）- 学习笔记](TheBeautyOfSoftwareEngineering.md)
- 下一篇 -> [UML 统一建模语言](UML.md)
