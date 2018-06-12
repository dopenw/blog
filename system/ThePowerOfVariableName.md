# 变量名的力量


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [变量名的力量](#变量名的力量)
	* [最重要的命名注意事项](#最重要的命名注意事项)
	* [以问题为导向](#以问题为导向)
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

## 链接
* 《代码大全 2rd》 第 11 章

[上一级](base.md)
[上一篇](GreedyAlgorithms.md)
[下一篇](UML.md)
