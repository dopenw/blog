# boost 程序库的简单使用

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [boost 程序库的简单使用](#boost-程序库的简单使用)
  - [官网](#官网)
  - [Boost.Any](#boostany)

<!-- /code_chunk_output -->

## 官网

[Boost.org](https://www.boost.org/)

## Boost.Any

[Boost.Any](https://www.boost.org/doc/libs/1_74_0/doc/html/any.html)

example:

```c++
#include <list>
#include <boost/any.hpp>
#include <iostream>

using boost::any_cast;

bool is_empty(const boost::any &operand) {
 return operand.empty();
}

bool is_int(const boost::any &operand) {
 return operand.type() == typeid(int);
}

bool is_char_ptr(const boost::any &operand) {
 try {
  any_cast<const char*>(operand);
  return true;
 } catch (const boost::bad_any_cast&) {
  return false;
 }
}

bool is_string(const boost::any &operand) {
 return any_cast<std::string>(&operand);
}

void count_all(const std::list<boost::any> &values, std::ostream &out) {
 out << "#empty == " << std::count_if(values.begin(), values.end(), is_empty)
   << std::endl;
 out << "#int == " << std::count_if(values.begin(), values.end(), is_int)
   << std::endl;
 out << "#const char * == "
   << std::count_if(values.begin(), values.end(), is_char_ptr)
   << std::endl;
 out << "#string == "
   << std::count_if(values.begin(), values.end(), is_string)
   << std::endl;
}

int main(int argc, char **argv) {
 std::list<boost::any> many;
 many.push_back(1);
 many.push_back("hello");
 many.push_back(std::string("World"));
 many.push_back(2.3);
 count_all(many, std::cout);
 return 0;
}
```

Run it:

```sh
#empty == 0
#int == 1
#const char * == 1
#string == 1
```

使用场景1:

```c++
struct Property
{
    Property();
    Property(const std::string &, const boost::any &);

    std::string name;
    boost::any value;
};

typedef std::list<Property> Properties;
```

使用场景2：

```c++
class Consumer
{
public:
    virtual void notify(const boost::any &) = 0;
    ...
};
```

[上一级](README.md)
[上一篇](algorithmSortNonStaticMemberFunction.md)
[下一篇](boostFileSystem.md)
