# 变量名的力量


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [变量名的力量](#变量名的力量)
	* [最重要的命名注意事项](#最重要的命名注意事项)
	* [以问题为导向](#以问题为导向)
	* [最适当的名字长度](#最适当的名字长度)
	* [与语言相关的命名规则的指导原则](#与语言相关的命名规则的指导原则)
		* [C 的命名规则](#c-的命名规则)
		* [C++ 的命名规则](#c-的命名规则-1)
		* [Java 的规则](#java-的规则)
	* [混合语言编程的注意事项](#混合语言编程的注意事项)
	* [命名规则示例](#命名规则示例)
	* [链接](#链接)

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

[上一级](base.md)
[上一篇](GreedyAlgorithms.md)
[下一篇](UML.md)
