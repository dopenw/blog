# Google c++ test framework


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Google c++ test framework](#google-c-test-framework)
	* [install gtest](#install-gtest)
	* [basic Assertions](#basic-assertions)
	* [Binary Comparison](#binary-comparison)
	* [String Comparison](#string-comparison)
	* [create test project](#create-test-project)
	* [run test project](#run-test-project)

<!-- /code_chunk_output -->


## install gtest
```sh
git clone https://github.com/google/googletest.git
cd googletest
cmake CMakeLists.txt
make
sudo make install
```

## basic Assertions
| 致命的断言    | 非致命的断言| 验证
| :------------- | :------------- |-|
|ASSERT_TRUE(condition);	|EXPECT_TRUE(condition);	|condition is true
|ASSERT_FALSE(condition);	|EXPECT_FALSE(condition); |condition is false

## Binary Comparison
| 致命的断言    | 非致命的断言| 验证
|-|-|-|
|ASSERT_EQ(val1,val2);	|EXPECT_EQ(val1,val2);	|val1 == val2
|ASSERT_NE(val1,val2);	|EXPECT_NE(val1,val2);	|val1 != val2
|ASSERT_LT(val1,val2);	|EXPECT_LT(val1,val2);	|val1 < val2
|ASSERT_LE(val1,val2);	|EXPECT_LE(val1,val2);	|val1 <= val2
|ASSERT_GT(val1,val2);	|EXPECT_GT(val1,val2);	|val1 > val2
|ASSERT_GE(val1,val2);	|EXPECT_GE(val1,val2);	|val1 >= val2

## String Comparison
| 致命的断言    | 非致命的断言| 验证
|-|-|-|
ASSERT_STREQ(str1,str2);|	EXPECT_STREQ(str1,str2);	|the two C strings have the same content
ASSERT_STRNE(str1,str2);|	EXPECT_STRNE(str1,str2);	|the two C strings have different content
ASSERT_STRCASEEQ(str1,str2);|	EXPECT_STRCASEEQ(str1,str2);	|the two C strings have the same content, ignoring case
ASSERT_STRCASENE(str1,str2);|	EXPECT_STRCASENE(str1,str2);	|the two C strings have different content, ignoring case

## create test project

```c++
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

double squareRoot(const double a) {
  double b = sqrt(a);
  if (b != b) { // nan check
    return -1.0;
  } else {
    return sqrt(a);
  }
}

TEST(SquareRootTest, PositiveNos) {
  EXPECT_EQ(18.0, squareRoot(324.0));
  EXPECT_EQ(25.4, squareRoot(645.16));
  ASSERT_EQ(50.3321, squareRoot(2533.310224));
}

TEST(SquareRootTest, NegativeNos) {
  ASSERT_EQ(-1.0, squareRoot(-16.0));
  ASSERT_EQ(-1.0, squareRoot(-0.2));
}
TEST(i, j) { ASSERT_TRUE(false); }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

## run test project
编译：
```sh
g++ -lpthread -lgtest test.cpp -o test
```
运行结果：
```sh
[==========] Running 3 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 2 tests from SquareRootTest
[ RUN      ] SquareRootTest.PositiveNos
test.c:19: Failure
      Expected: 50.3321
To be equal to: squareRoot(2533.310224)
      Which is: 50.332
[  FAILED  ] SquareRootTest.PositiveNos (0 ms)
[ RUN      ] SquareRootTest.NegativeNos
[       OK ] SquareRootTest.NegativeNos (0 ms)
[----------] 2 tests from SquareRootTest (0 ms total)

[----------] 1 test from i
[ RUN      ] i.j
test.c:26: Failure
Value of: false
  Actual: false
Expected: true
[  FAILED  ] i.j (0 ms)
[----------] 1 test from i (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 2 test cases ran. (0 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 2 tests, listed below:
[  FAILED  ] SquareRootTest.PositiveNos
[  FAILED  ] i.j

 2 FAILED TESTS
```

[gtest doc](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md)
[gtest quick introduction](https://www.ibm.com/developerworks/aix/library/au-googletestingframework.html#list1)
[上一级](base.md)
[上一篇](function_arg_stack.md)
[下一篇](initalization_list.md)
