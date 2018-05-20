# Google c++ test framework


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Google c++ test framework](#google-c-test-framework)
	* [install gtest](#install-gtest)
	* [basic Assertions](#basic-assertions)
	* [Binary Comparison](#binary-comparison)
	* [String Comparison](#string-comparison)
	* [Floating-Point Comparison](#floating-point-comparison)
	* [create test project](#create-test-project)
	* [run test project](#run-test-project)
	* [run gtest sample unit test](#run-gtest-sample-unit-test)
	* [Test Fixtures](#test-fixtures)

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

## Floating-Point Comparison

Due to round-off errors, it is very unlikely that two floating-points will match exactly. Therefore, ASSERT_EQ 's naive comparison usually doesn't work.

| Fatal assertion | Nonfatal assertion     |  Verifies |
| :------------- | :------------- | -|
| ASSERT_FLOAT_EQ(val1,val2)       | EXPECT_FLOAT_EQ(val1,val2)       |  the two float values are almost equal |
| ASSERT_DOUBLE_EQ(val1,val2) | EXPECT_DOUBLE_EQ(val1,val2) | the two values are almost equal |

The following assertions allow you to choose the acceptable error bound:

| **Fatal assertion** | **Nonfatal assertion** | **Verifies** |
|:--------------------|:-----------------------|:-------------|
| `ASSERT_NEAR(`_val1, val2, abs\_error_`);` | `EXPECT_NEAR`_(val1, val2, abs\_error_`);` | the difference between _val1_ and _val2_ doesn't exceed the given absolute error |

[Floating-Point Comparison](https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#floating-point-macros)

[Error with EXPECT_EQ for sum of double or float
](https://stackoverflow.com/questions/15128510/error-with-expect-eq-for-sum-of-double-or-float)


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
	EXPECT_DOUBLE_EQ(18.0, squareRoot(324.0));
	EXPECT_DOUBLE_EQ(25.4, squareRoot(645.16));
	ASSERT_DOUBLE_EQ(50.3321, squareRoot(2533.310224));
}

TEST(SquareRootTest, NegativeNos) {
	ASSERT_DOUBLE_EQ(-1.0, squareRoot(-16.0));
  ASSERT_DOUBLE_EQ(-1.0, squareRoot(-0.2));
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
g++ test.cpp -lpthread -lgtest  -o test
```
运行结果：
```sh
[breap@breap cpp]$ ./test
[==========] Running 3 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 2 tests from SquareRootTest
[ RUN      ] SquareRootTest.PositiveNos
test.cpp:19: Failure
      Expected: 50.3321
      Which is: 50.332099999999997
To be equal to: squareRoot(2533.310224)
      Which is: 50.332000000000001
[  FAILED  ] SquareRootTest.PositiveNos (0 ms)
[ RUN      ] SquareRootTest.NegativeNos

[       OK ] SquareRootTest.NegativeNos (0 ms)
[----------] 2 tests from SquareRootTest (0 ms total)

[----------] 1 test from i
[ RUN      ] i.j
test.cpp:26: Failure
Value of: false
  Actual: false
Expected: true
[  FAILED  ] i.j (0 ms)
[----------] 1 test from i (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 2 test cases ran. (1 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 2 tests, listed below:
[  FAILED  ] SquareRootTest.PositiveNos
[  FAILED  ] i.j

 2 FAILED TESTS
```

[gtest doc](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md)

[gtest quick introduction](https://www.ibm.com/developerworks/aix/library/au-googletestingframework.html#list1)


## run gtest sample unit test

[gtest sample](https://github.com/google/googletest/blob/master/googletest/docs/Samples.md)

```sh
git clone https://github.com/google/googletest.git
cd googletest
cmake CMakeLists.txt
make
cd ($googleTestDir)/googletest/make
make
```
这样就会编译出可执行文件 sample1_unittest

运行即可得到：
```sh
[==========] Running 6 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 3 tests from FactorialTest
[ RUN      ] FactorialTest.Negative
[       OK ] FactorialTest.Negative (0 ms)
[ RUN      ] FactorialTest.Zero
[       OK ] FactorialTest.Zero (0 ms)
[ RUN      ] FactorialTest.Positive
[       OK ] FactorialTest.Positive (0 ms)
[----------] 3 tests from FactorialTest (0 ms total)

[----------] 3 tests from IsPrimeTest
[ RUN      ] IsPrimeTest.Negative
[       OK ] IsPrimeTest.Negative (0 ms)
[ RUN      ] IsPrimeTest.Trivial
[       OK ] IsPrimeTest.Trivial (0 ms)
[ RUN      ] IsPrimeTest.Positive
[       OK ] IsPrimeTest.Positive (0 ms)
[----------] 3 tests from IsPrimeTest (0 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 2 test cases ran. (0 ms total)
[  PASSED  ] 6 tests.
```

也可直接在sample目录下面直接运行：

```sh
g++ sample3_unittest.cc ../src/gtest_main.cc -lpthread -lgtest -o sample3
```

run sample3:

```terminal
[==========] Running 3 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 3 tests from QueueTestSmpl3
[ RUN      ] QueueTestSmpl3.DefaultConstructor
[       OK ] QueueTestSmpl3.DefaultConstructor (0 ms)
[ RUN      ] QueueTestSmpl3.Dequeue
[       OK ] QueueTestSmpl3.Dequeue (0 ms)
[ RUN      ] QueueTestSmpl3.Map
[       OK ] QueueTestSmpl3.Map (0 ms)
[----------] 3 tests from QueueTestSmpl3 (0 ms total)
[----------] Global test environment tear-down
[==========] 3 tests from 1 test case ran. (0 ms total)
[PASSED] 3 tests
```

## Test Fixtures

[gtest test fixtures github md](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md#test-fixtures-using-the-same-data-configuration-for-multiple-tests)


* SetUp() will be called before each test is run.  You should define it if you need to initialize the variables. Otherwise, this can be skipped.

* If necessary, write a destructor or TearDown() function to release any resources you allocated in SetUp()

When using a fixture, use TEST_F() instead of TEST() as it allows you to access objects and subroutines in the test fixture:

```c++
TEST_F(test_case_name, test_name) {
 ... test body ...
}
```

For each test defined with TEST_F(), Google Test will:

1. Create a fresh test fixture at runtime
2. Immediately initialize it via SetUp()
3. Run the test
4. Clean up by calling TearDown()
5. Delete the test fixture. Note that different tests in the same test case have different test fixture objects, and Google Test always deletes a test fixture before it creates the next one. Google Test does not reuse the same test fixture for multiple tests. Any changes one test makes to the fixture do not affect other tests.


[Google Test Fixtures Ask Question in stack overflow ](https://stackoverflow.com/questions/3549540/google-test-fixtures)

[csdn link](http://blog.csdn.net/russell_tao/article/details/7333226)


[上一级](base.md)
[上一篇](googleProtobuf.md)
[下一篇](initalization_list.md)
