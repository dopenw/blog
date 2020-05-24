# 开发者测试


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [开发者测试](#开发者测试)
  - [开发者测试在软件质量中的角色](#开发者测试在软件质量中的角色)
  - [开发者测试的推荐方法](#开发者测试的推荐方法)
    - [测试先行还是测试后行](#测试先行还是测试后行)
    - [开发者测试的局限性](#开发者测试的局限性)
  - [测试技巧锦囊](#测试技巧锦囊)
    - [不完整的测试](#不完整的测试)
    - [结构化的基础测试](#结构化的基础测试)
    - [数据流测试](#数据流测试)
    - [等价类划分](#等价类划分)

<!-- /code_chunk_output -->

测试是常见的改善质量的活动：
* `单元测试（Unit testing）`是将一个程序员或者一个开发团队所编写的，一个完整的类、子程序或者小程序，从完整的系统中隔离出来进行测试。
* `组件测试（Component testing）`是将一个类、包、小程序或者其他程序元素，从一个更加完整的系统中隔离出来进行测试，这些被测代码涉及到多个程序员或者多个团队。
* `集成测试（Integration testing）` 是对两个或更多的类、包、组件或者子系统进行的联合测试，这些组件由多个程序员或者开发团队所创建。这种测试通常在有了两个可以进行测试的类的时候就应该尽快开始，并且一直持续到整个系统开发完成。
* `回归测试（Regression testing）` 是指重复执行以前的测试用例，以便在原先通过了相同测试集合的软件中查找缺陷。
* `系统测试（System testing）` 是在最终的配置下（包括同其他软硬件系统的集成）运行整个软件。以便测试安全、性能、资源消耗、时序方面的问题，以及其他无法在低级集成上测试的问题。

测试通常分为两大类：黑盒测试 (black-box testing) 和 白盒 (white-box testing,或者 玻璃盒测试 glass-box testing)。
* 黑盒测试：指的是测试者无法了解测试对象内部工作机制的测试。
* 白盒测试：指的是测试者清楚待测对象内部工作机制的测试。
本章关注的是白盒测试，即由开发者完成的测试。

“测试” 和 “调试” 是不同的术语：
* 测试是一种检查错误的方法
* 调试意味着错误已经被发现，要做的是诊断错误并消灭造成这些错误的根本原因。

## 开发者测试在软件质量中的角色
对于任何软件质量规划来说，测试都是一个重要的组成部分，并且在许多情况下它是唯一的组成部分。这是非常不幸的，因为各种形式的协同开发实践都表现出比测试更高的错误检查率，而且发现一条测试的成本不到测试的二分之一。

测试对于绝大数开发人员来说都是一种煎熬：
* 测试的目标与开发活动背道而驰，测试的目标是找出错误。一个成功的测试应该弄跨软件，而其他开发活动的目标是避免程序错误和软件的崩溃。
* 测试永远不可能彻底证明程序中没有错误。
* 测试本身并不能改善软件的质量。测试的结果是软件质量的一个指示器，但是结果本身并不改善软件质量。
* 测试是要求你假设会在代码里面找到错误。

如下图所示，根据项目大小和复杂程度的不同，开发者测试应该占整个项目时间的 8% ~ 25%.

![](../images/developTest_201809011647_1.png)

理解测试基本概念可以更好地支持测试，也能提供测试的效率。

## 开发者测试的推荐方法
采用系统化的开发者测试方法，能最大限度提高你发现各种错误的能力，同时让你的花费也最少。请确保下面所有要点你都能做到：
* 对每一项相关的需求进行测试，以确保需求都已经被实现。在需求阶段就计划好这一部分的测试用例，或者至少尽早开始-最好在你开始编写代测试的单元之前。注意对需求里面常见的疏漏进行测试。安全级别、数据存储、安装过程以及系统可靠性等，这些都是测试的绝佳素材，并且常常在需求阶段被忽略。
* 对每一个相关的设计关注点进行测试，以确保设计已经被实现。
* 用基础测试（basis testing）来扩充针对需求和设计的详细测试用例。增加数据流测试（data-flow testing），然后补充其他所需的测试用例，以便对代码进行彻底的考验。至少你应该测试到每一行代码。
* 使用一个检查表，其中记录着你在项目迄今为止所犯的，以及在过去的项目中所犯的错误类型。

在设计产品的时候设计测试用例，这样可以帮助避免在需求和设计中产生错误，修正这些错误的代价往往比修正编码错误更昂贵。越早修复这些缺陷，成本就越低，因此，要尽可能地对测试进行规划并找出缺陷。

### 测试先行还是测试后行
通过下面的缺陷成本增长图，我们可以得到：首先写测试用例可以将从引入缺陷到发现并排除缺陷之间的时间缩减至最短。这正是首先写测试用例的诸多原因之一。

![](../images/developTest_201809011941_1.png)

* 在开始写代码先写测试用例，并不比之后在写要多花功夫，只是调整了一下测试用例编写活动的工作顺序而已。
* 假如你首先编写测试用例，那么你将可以更早发现缺陷，同时也更容易修正它们。
* 首先编写测试用例，将迫使你在开始写代码之前至少思考一下需求和设计，而这往往会催生更高质量的代码。
* 首先编写测试用例，能更早地把需求上的问题暴露出来，因为对于一个糟糕的需求来说，要写出测试用例是一件困难的事情。
* 如果你保存了最初的测试用例-这是你应该做的，那么先进行测试并非唯一选择，你仍然可以最后在进行测试。

总而言之，我认为测试先行的编程是过去十年中所形成的最有用的软件开发实践之一，同时也是一个非常好的通用方法。但这并不是一种测试万能药，正如接下来将要说明的那样，它同样受制于开发者测试本身的局限性。


### 开发者测试的局限性
应注意到开发者测试的下述局限性：
* 开发者测试倾向于 “干净测试”：开发人员往往去做一些检查代码能否工作的测试（干净测试，clean test）,而不是所用可能让代码失效的测试（肮脏测试，dirty test）。
* 开发者测试对覆盖率由过于乐观的估计: 平均而言，，程序员坚信它们的测试覆盖率达到了 95% ,但通常，最佳情况下这一数字也只能达到大约 80%，而在最糟糕的情况下只有 30%。平均而言大约在 50% ~ 60%.
* 开发者测试往往会忽略一些更复杂的测试覆盖率类型

上面的这些讨论绝不是想要贬低开发者测试的价值，相反，它们促使我们用正确的观点来看待开发者测试。开发者测试是有价值的，但对于提供足够的质量保证而言，仅仅进行开发者测试是不够的。我们需要补充其他的测试，包括独立测试(independent testing) 技术以及协同构建 (collaborative construction) 技术。

## 测试技巧锦囊

为什么说通过测试来证明程序的正确性是不可能的呢？如果要用测试来证明一个程序的正确性，你需要对程序的每一种可能的输入值，以及它们之间的所有的可以想像的组合进行测试。即使是一个简单的程序，这样庞大的任务都会让人望而却步。

### 不完整的测试
由于进行完全测试实际上是不可能的，因此测试的窍门就在于选择那些最有可能找到错误的测试用例。

### 结构化的基础测试

其思想是，你需要去测试程序中的每一条语句至少一次。例如 if 语句或者 while 语句，那么你就需要根据 if 或者 while 中表达式的复杂程度来修改测试，以确保这个语句完全经过了测试。要确保你已经覆盖了所有的基础情况，最简单的方法就是算一算有多少条通过程序的路径，然后据此开发出能通过程序里每条路径的最少数量的测试用例。

测试穿过程序里的所有路径的两种方法：`代码覆盖测试`或`逻辑覆盖测试`。这两种方法覆盖了所有的路径，因此，它们和结构化的基础测试很相似，但是它们并不蕴涵者以最小数量的测试用例覆盖所有路径的思想。如果使用代码覆盖率测试或者逻辑覆盖测试，在覆盖相同逻辑的情况下，你需要创建的测试用例远多于结构化的基础测试。
所需基础测试用例的最少数量可以这样计算：
1. 对通过子程序的直路，开始的时候记 1.
2. 遇到下面的关键字或者其等价物时，加 1： if 、while、repeat、for、and 以及 or
3. 遇到每一个 case 语句就加 1,如果 case 语句没有缺省情况，则再加 1.

```java
// 记 1
Statement1;
Statement2;
if (x < 10) { // 遇到 if,记作 2
	Statement3;
}
Statement4;
```
对于这个例子，我们需要如下两个测试用例：
* 由 if 控制的语句执行 (x<10);
* 由 if 控制的语句不执行 (x>=10)

下面是一个稍微复杂一点的例子，这一段代码将在本章中不断使用，并且包含了一些可能的错误。
```java
//Example of Computing the Number of Cases Needed for Basis Testing
// Compute Net Pay
totalWithholdings = 0;
for ( id = 0; id < numEmployees; id++ ) {
	// compute social security withholding, if below the maximum
	if ( m_employee[ id ].governmentRetirementWithheld < MAX_GOVT_RETIREMENT ) {
		governmentRetirement = ComputeGovernmentRetirement( m_employee[ id ] );
	}
	// set default to no retirement contribution
	companyRetirement = 0;
	// determine discretionary employee retirement contribution
	if ( m_employee[ id ].WantsRetirement &&
		EligibleForRetirement( m_employee[ id ] ) ) {
		companyRetirement = GetRetirement( m_employee[ id ] );
	}
	grossPay = ComputeGrossPay ( m_employee[ id ] );
	// determine IRA contribution
	personalRetirement = 0;
	if ( EligibleForPersonalRetirement( m_employee[ id ] ) ) {
		personalRetirement = PersonalRetirementContribution( m_employee[ id ],
			companyRetirement, grossPay );
	}
	// make weekly paycheck
	withholding = ComputeWithholding( m_employee[ id ] );
	netPay = grossPay - withholding - companyRetirement - governmentRetirement –
	personalRetirement;
	PayEmployee( m_employee[ id ], netPay );
	// add this employee's paycheck to total for accounting
	totalWithholdings = totalWithholdings + withholding;
	totalGovernmentRetirement = totalGovernmentRetirement + governmentRetirement;
	totalRetirement = totalRetirement + companyRetirement;
}
SavePayRecords( totalWithholdings, totalGovernmentRetirement, totalRetirement );
```
下面是一套能够覆盖这一例子中所有基本情况的测试用例：
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-0pky{border-color:inherit;text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-c3ow">用例</th>
    <th class="tg-c3ow">测试用例描述</th>
    <th class="tg-c3ow">测试数据</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-c3ow">1</td>
    <td class="tg-c3ow">正常情况</td>
    <td class="tg-c3ow">所有布尔条件都为真</td>
  </tr>
  <tr>
    <td class="tg-c3ow">2</td>
    <td class="tg-c3ow">初始 for 条件为假</td>
    <td class="tg-c3ow">numEmployee &lt; 1</td>
  </tr>
  <tr>
    <td class="tg-c3ow">3 </td>
    <td class="tg-c3ow">第一个 if 为假</td>
    <td class="tg-c3ow">m_employee[ id ].governmentRetirementWithheld &gt;= MAX_GOVT_RETIREMENT</td>
  </tr>
  <tr>
    <td class="tg-c3ow">4</td>
    <td class="tg-c3ow">第二个 if由于 and之前的部分为假，结果为假</td>
    <td class="tg-c3ow">not&nbsp;&nbsp;m_employee[ id ].WantsRetirement</td>
  </tr>
  <tr>
    <td class="tg-c3ow">5</td>
    <td class="tg-c3ow">第二个 if由于 and后半部分为假，结果为假</td>
    <td class="tg-c3ow">not EligibleForRetirement( m_employee[ id ] )</td>
  </tr>
  <tr>
    <td class="tg-c3ow">6</td>
    <td class="tg-c3ow">第三个 if 为假</td>
    <td class="tg-c3ow">not EligibleForPersonalRetirement( m_employee[ id ] )</td>
  </tr>
  <tr>
    <td class="tg-0pky" colspan="3">注意：在本章中，我们会不断地对该表增添更多的测试用例</td>
  </tr>
</tbody>
</table>

现在你已经为该子程序创建了 6 个测试用例，这满足了结构化的基础测试的要求，那么你认为这一个子程序已经经过完整的测试了吗？很可能并非如此。
这种测试能够向你保证所有的代码都得到执行，但它并不能说明数据的变化情况。

### 数据流测试
将上一节和本节的内容综合起来考虑，给出了另一个例子，该例显示控制流和数据流在计算机程序设计中的重要性部分伯仲。
数据流测试基于如下观念：数据使用的出错几率至少不亚于控制流。Boris Beizer 声称全部代码中至少有一半是数据声明和初始化。
数据的状态可以是下列三种状态中的一种：
* 已定义 - 数据已经初始化，但是还没有使用
* 已使用 - 数据已经用于计算，或作为某子程序调用的一个参数，或者用于其他用途。
* 已销毁 - 数据曾经定义过，但是现在已经通过某种途径取消了对它的定义。

为了方便起见，还应该有一些术语来描述对变量进行某种操作之前或者之后，控制流进入或退出某个子程序的状态。
* 已进入 - 控制流已经进入一个子程序，但还没有使用该变量。例如一个在子程序中使用的变量在子程序开始处进行初始化。
* 已退出 - 在对变量产生影响之后，控制流立即推出子程序。例如在子程序的结尾处把返回值赋给一个状态变量。

数据状态的组合:
* 已定义 - 已定义  其实这是多此一举，就算当时没有出错，也是非常危险的。
* 已定义 - 已退出 如果这是一个局部变量，那完全没有理由在定义后却不使用就退出。如果是一个子程序的参数或者全局变量，这还说得过去。
* 已定义 - 已销毁 定义一个变量然后就销毁它，这意味着要么这个变量对程序没什么意义，要么就是程序员忘记写使用这个变量的代码了。
* 已进入 - 已销毁 如果这是一个局部变量，那就有问题了。如果局部变量未定义或者未使用，那么它完全不需要被销毁。而另一方面，如果这是一个子程序的参数或者全局变量，只要在销毁之前确定该变量已经在别的什么地方定义过，那么使用这种组合就没有问题。
* 已进入 - 已使用 还是一样，如果这是一个局部变量就有问题。这个变量应该在使用之前被定义。反之，如果这是一个子程序的参数或者是一个全局变量，那么只要这个变量在使用之前已经在别的地方被定义过，那么就没有任何问题。
* 已销毁 - 已销毁 一个变量不应该被销毁两次
* 已销毁 - 已使用 使用一个已销毁的变量，从逻辑上就讲不通。如果这样的代码似乎还能正常工作，这只是一种例外的情况，墨菲定律（任何可能出错的事都会出错）也说代码如果停止工作将要引起极大混乱，那这时它就会停止工作。
* 已使用 - 已定义 对一个变量先使用后定义，可能是问题也可能不是，这就要看这个变量在使用之前是否已经定义过。当然，如果你看到某个已使用 - 已定义的模式，那么检查一下之前的定义是非常有必要的。

在开始测试之前，首先要检查一下，看看是否出现了这些反常的数据状态顺序。在做过这些检查之后，编写数据流测试用例的关键是要对所有可能的定义-使用路径进行测试。你可以采用彻底程度不同的测试。
* 所有定义。测试每一个变量的每一个定义 - 在每个变量被赋值的地方。这是一个很弱的策略，因为如果你曾经尝试对每一行代码进行测试，那么就已经做过这件事了。
* 使用已定义 - 已使用的组合。对每一个变量测试所有在某处定义而在另一处使用的组合，与测试所有定义相比，这是一个更强的策略，因为仅仅执行每一行代码并不能保证测试到每一种已定义-已使用的组合。


下面是一个例子：
```java
//Java Example of a Program Whose Data Flow Is to Be Tested
if (Condition 1) {
	x=a;
}
else
{
	x=b;
}
if (Condition 2) {
	y=x+1;
}
else
{
	y=x-1;
}
```
结合上面一节说的 结构化的基础测试，我们可以得到两个测试用例：
1. Condition 1 true,Condition 2 true
2. Condition 1 false ,Condition 2 false
要测试每一种已定义-已使用的组合，你还需要另外两种情况：
3. Condition 1 true,Condition 2 false
4. Condition 1 false,Condition 2 true

开发测试用例的一个好方法是首先进行结构化的基础测试，即使它没有测试所有的已定义-已使用数据流的形式，但至少也完成了其中的一部分。然后你需要添加完整的已定义-已使用数据流测试所需的用例。

正如前一节所讨论的那样，结构化的基础测试已经为其提供了 6 个测试用例。补充所有的数据流组合：

| 用例 	|                                       测试用例描述                                      	|
|:----:	|:---------------------------------------------------------------------------------------:	|
|   7  	| 在第12行定义 companyRetirement,并在第 26 行首次使用它，前面的测试用例并没有覆盖这种情况 	|
|   8  	| 在第12行定义 companyRetirement,并在第 31 行首次使用它，前面的测试用例并没有覆盖这种情况 	|
|   9  	| 在第17行定义 companyRetirement,并在第 31 行首次使用它，前面的测试用例并没有覆盖这种情况 	|

### 等价类划分
一个好的测试用例应该覆盖可输入数据中的很大一部分。如果两个用例能揭示的错误完全相同，那么只要一个就够了。“等价类划分”的概念是这一想法的严格表现形式，应用它有助于减少所需用例的数量。

在上一节中的用例表中，测试用例 3 是使用等价类划分的好地方，在这一情况下，有两个等价类：
* m_employee[ id ].governmentRetirementWithheld >= MAX_GOVT_RETIREMENT
* m_employee[ id ].governmentRetirementWithheld <
 MAX_GOVT_RETIREMENT

如果你已经通过基础测试和数据流覆盖了整个程序，那么对等价类划分的研究并不会对你的程序产生多少新的认识。然而，如果你从程序的外部来审视（从规格说明而非源代码角度），或是数据很复杂且程序的逻辑并为完全体现这种复杂性的时候，等价类划分还是大有好处的。


注： 读 《代码大全 2rd》 第 22 章 开发者测试，学习笔记

[上一级](README.md)
[上一篇](c++VSjava.md)
[下一篇](dynamicProgramming.md)
