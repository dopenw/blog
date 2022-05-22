<!-- C++ Preprocessor -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [Preprocessor](#preprocessor)
- [Sample 1](#sample-1)
- [Sample 2](#sample-2)
- [Sample 3](#sample-3)
- [Sample 4](#sample-4)

<!-- /code_chunk_output -->


## Preprocessor

[tutorialsPoint:C++ Preprocessor](https://www.tutorialspoint.com/cplusplus/cpp_preprocessor.htm)

## Sample 1

test.h:
```c++
#ifndef TEST_H_
#define TEST_H_

#define MACRO_TEST

#endif /* TEST_H_ */
```

test.cpp
```c++
#include <iostream>
#include "test.h"


int main(int argc, char **argv) {

#ifdef MACRO_TEST
	std::cout<<"test ok"<<std::endl;
#else
	std::cout<<"test failed"<<std::endl;
#endif

	return 0;
}
```

Output:
```highlight
test ok
```

## Sample 2
test.h:
```c++
#ifndef TEST_H_
#define TEST_H_

void test()
{
#ifdef MACRO_TEST
	std::cout<<"test ok"<<std::endl;
#else
	std::cout<<"test failed"<<std::endl;
#endif
}

#endif /* TEST_H_ */
```

test.cpp:
```c++
#include <iostream>
#include "test.h"

#define MACRO_TEST

int main(int argc, char **argv) {
  test();
	return 0;
}
```

Output:
```highlight
test failed
```

## Sample 3
test.h:
```c++
#ifndef TEST_H_
#define TEST_H_

void test()
{
#ifdef MACRO_TEST
	std::cout<<"test ok"<<std::endl;
#else
	std::cout<<"test failed"<<std::endl;
#endif
}

#endif /* TEST_H_ */
```

test.cpp:
```c++
#include <iostream>
#define MACRO_TEST
#include "test.h"
#undef MACRO_TEST


int main(int argc, char **argv) {
	test();
	return 0;
}
```

```highlight
test ok
```

预处理后的代码：
```sh
[breap@localhost]$ g++ -E test.cpp
# 还有部分代码，在此处省略
# 2 "test.cpp" 2

# 1 "test.h" 1
# 11 "test.h"

# 11 "test.h"
void test()
{

 std::cout<<"test ok"<<std::endl;



}
# 4 "test.cpp" 2



int main(int argc, char **argv) {
 test();
 return 0;
}
```

## Sample 4
test.h:
```c++
#ifndef TEST_H_
#define TEST_H_

void test();

#endif /* TEST_H_ */
```

test.cpp:
```c++
#include "test.h"
#include <iostream>

void test()
{
#ifdef MACRO_TEST
	std::cout<<"test ok,test"<<std::endl;
#else
	std::cout<<"test failed"<<std::endl;
#endif
}
```

main.cpp:
```c++
#include <iostream>
#define MACRO_TEST
#include "test.h"
#undef MACRO_TEST


int main(int argc, char **argv) {
	test();
	return 0;
}
```


Output:
```highlight
test failed
```

```sh
[breap@localhost]$ g++ -E main.cpp
# 还有部分代码，在此处省略
# 2 "main.cpp" 2

# 1 "test.h" 1




# 4 "test.h"
void test();
# 4 "main.cpp" 2



int main(int argc, char **argv) {
 test();
 return 0;
}
```

```sh
[breap@localhost]$ g++ -E test.cpp
# 还有部分代码，在此处省略
# 3 "test.cpp" 2


# 4 "test.cpp"
void test()
{



 std::cout<<"test failed"<<std::endl;

}
```
---
- [上一级](README.md)
- 上一篇 -> [MFC 对比 QT](MFC_VS_QT.md)
- 下一篇 -> [Put functions into vector and execute](PutFunctionsIntoVectorAndExecute.md)
