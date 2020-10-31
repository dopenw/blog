# 一些 Boost 程序库的简单使用

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [一些 Boost 程序库的简单使用](#一些-boost-程序库的简单使用)
  - [官网](#官网)
  - [Any](#any)
  - [Format](#format)
  - [Lexical_Cast](#lexical_cast)
  - [Numeric Conversion](#numeric-conversion)
  - [Process](#process)
  - [Smart Ptr](#smart-ptr)

<!-- /code_chunk_output -->

## 官网

[Boost.org](https://www.boost.org/)

## Any

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

## Format

[Boost Format library](https://www.boost.org/doc/libs/1_74_0/libs/format/doc/format.html)

```c++
#include <boost/format.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

int main(int argc, char **argv) {
 using namespace std;
 using boost::format;
 using boost::io::group;

 cout << format("%1% %2% %3% %2% %1% \n")
   % "11" % "22" % "333"; // 'simple' style
 //It prints : "11 22 333 22 11 \n"

 //More precise formatting, with Posix-printf positional directives
 cout << format("(x,y) = (%1$+5d,%2$+5d) \n") % -23 % 35;     // Posix-Printf style
 //It prints : "(x,y) = (  -23,  +35) \n"

 //classical printf directive, no reordering
 cout << format("writing %s,  x=%s : %d-th step \n") % "toto" % 40.23 % 50;
 //It prints : "writing toto,  x=40.23 : 50-th step \n"

 //Several ways to express the same thing :
 cout << format("(x,y) = (%+5d,%+5d) \n") % -23 % 35;
 cout << format("(x,y) = (%|+5|,%|+5|) \n") % -23 % 35;
 cout << format("(x,y) = (%1$+5d,%2$+5d) \n") % -23 % 35;
 cout << format("(x,y) = (%|1$+5|,%|2$+5|) \n") % -23 % 35;
 // all those print : "(x,y) = ( -23, +35) \n"

 //Using manipulators to modify the format-string :
 format fmter("_%1$+5d_ %1$d \n");
 format fmter2("_%1%_ %1% \n");
 fmter2.modify_item(1, group(showpos, setw(5)) );
 cout << fmter % 101 ;
 cout << fmter2 % 101 ;
 //Both print the same : "_ +101_ 101 \n"

 //Using manipulators with arguments
 cout << format("_%1%_ %1% \n") % group(showpos, setw(5), 101);
 //it prints : "_ +101_  +101 \n"

 //New formatting feature : 'absolute tabulations'
 //, useful inside loops, to insure a field is printed
 //at the same position from one line to the next
 //, even if the widths of the previous arguments can vary a lot.
 std::vector<std::string> names={"Marc-François Michel","Jean"};
 std::vector<std::string> surname={"Durand","de Lattre de Tassigny"};
 std::vector<std::string> tel={"+33 (0) 123 456 789","+33 (0) 987 654 321"};

 for(unsigned int i=0; i < names.size(); ++i)
     cout << format("%1%, %2%, %|40t|%3%\n") % names[i] % surname[i] % tel[i];
 // it prints:
 //"Marc-François Michel, Durand,          +33 (0) 123 456 789"
 //"Jean, de Lattre de Tassigny,            +33 (0) 987 654 321"
}

```

Run it:

```sh
11 22 333 22 11
(x,y) = (  -23,  +35)
writing toto,  x=40.23 : 50-th step
(x,y) = (  -23,  +35)
(x,y) = (  -23,  +35)
(x,y) = (  -23,  +35)
(x,y) = (  -23,  +35)
_ +101_ 101
_ +101_ 101
_ +101_  +101
Marc-François Michel, Durand,          +33 (0) 123 456 789
Jean, de Lattre de Tassigny,            +33 (0) 987 654 321
```

## Lexical_Cast

[Boost.Lexical_Cast](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_lexical_cast.html)

* string to numbers conversion

```c++
#include <boost/lexical_cast.hpp>
#include <vector>

int main(int /*argc*/, char * argv[])
{
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    std::vector<short> args;

    while (*++argv)
    {
        try
        {
            args.push_back(lexical_cast<short>(*argv));
        }
        catch(const bad_lexical_cast &)
        {
            args.push_back(0);
        }
    }

    // ...
}
```

* Numbers to strings conversion

```c++
void log_message(const std::string &);

void log_errno(int yoko)
{
    log_message("Error " + boost::lexical_cast<std::string>(yoko) + ": " + strerror(yoko));
}
```

* Converting to string without dynamic memory allocation

```c++
void number_to_file(int number, std::FILE* file)
{
    typedef boost::array<char, 50> buf_t; // You can use std::array if your compiler supports it
    buf_t buffer = boost::lexical_cast<buf_t>(number); // No dynamic memory allocation
    std::fputs(buffer.begin(), file);
}
```

* Converting part of the string

```c++
int convert_strings_part(const std::string& s, std::size_t pos, std::size_t n)
{
    return boost::lexical_cast<int>(s.data() + pos, n);
}
```

## Numeric Conversion

[Boost.NumericConversion](https://www.boost.org/doc/libs/1_74_0/libs/numeric/conversion/doc/html/index.html)

The Boost Numeric Conversion library is a collection of tools to describe and perform conversions between values of different numeric types.

The library includes a special alternative for a subset of [std::numeric_limits<>](https://www.boost.org/doc/libs/1_74_0/libs/numeric/conversion/doc/html/boost_numericconversion/improved_numeric_cast__.html), the [bounds<>](https://www.boost.org/doc/libs/1_74_0/libs/numeric/conversion/doc/html/boost_numericconversion/bounds___traits_class.html) traits class, which provides a consistent way to obtain the boundary values for the range of a numeric type.

example:

```c++
int main()
{
    using boost::numeric_cast;

    using boost::numeric::bad_numeric_cast;
    using boost::numeric::positive_overflow;
    using boost::numeric::negative_overflow;

    try
    {
        int i=42;
        short s=numeric_cast<short>(i); // This conversion succeeds (is in range)
    }
    catch(negative_overflow& e) {
        std::cout << e.what();
    }
    catch(positive_overflow& e) {
        std::cout << e.what();
    }

    try
    {
        float f=-42.1234;

        // This will cause a boost::numeric::negative_overflow exception to be thrown
        unsigned int i=numeric_cast<unsigned int>(f);
    }
    catch(bad_numeric_cast& e) {
        std::cout << e.what();
    }

    double d= f + numeric_cast<double>(123); // int -> double

    unsigned long l=std::numeric_limits<unsigned long>::max();

    try
    {
        // This will cause a boost::numeric::positive_overflow exception to be thrown
        // NOTE: *operations* on unsigned integral types cannot cause overflow
        // but *conversions* to a signed type ARE range checked by numeric_cast.

        unsigned char c=numeric_cast<unsigned char>(l);
    }
    catch(positive_overflow& e) {
        std::cout << e.what();
    }


    return 0;
}
```

```c++
#include <iostream>

#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>

int main() {

    std::cout << "numeric::bounds versus numeric_limits example.\n";

    std::cout << "The maximum value for float:\n";
    std::cout << boost::numeric::bounds<float>::highest() << "\n";
    std::cout << std::numeric_limits<float>::max() << "\n";

    std::cout << "The minimum value for float:\n";
    std::cout << boost::numeric::bounds<float>::lowest() << "\n";
    std::cout << -std::numeric_limits<float>::max() << "\n";

    std::cout << "The smallest positive value for float:\n";
    std::cout << boost::numeric::bounds<float>::smallest() << "\n";
    std::cout << std::numeric_limits<float>::min() << "\n";

    return 0;
}
```

## Process

[Boost.Process](https://www.boost.org/doc/libs/1_74_0/doc/html/process.html)

Boost.Process is a library to manage system processes. It can be used to:

* create child processes
* setup streams for child processes
* communicate with child processes through streams (synchronously or asynchronously)
* wait for processes to exit (synchronously or asynchronously)
* terminate processes

example:

```c++
#include <boost/process.hpp>

#include <string>
#include <iostream>

using namespace boost::process;

int main()
{
    ipstream pipe_stream;
    child c("gcc --version", std_out > pipe_stream);

    std::string line;

    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
        std::cerr << line << std::endl;

    c.wait();
}
```

Run it:

```sh
gcc (GCC) 10.2.1 20201016 (Red Hat 10.2.1-6)
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## Smart Ptr

[Boost.SmartPtr](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html)

This library provides six smart pointer class templates:

* [scoped_ptr](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html#scoped_ptr), used to contain ownership of a dynamically allocated object to the current scope;it is noncopyable, it is safer than shared_ptr for pointers which should not be copied.scoped_ptr is a simple solution for simple needs.scoped_ptr cannot be used in C++ Standard Library containers. Use shared_ptr or std::unique_ptr if you need a smart pointer that can.

* [scoped_array](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html#scoped_array), which provides scoped ownership for a dynamically allocated array;

* [shared_ptr](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html#shared_ptr), a versatile tool for managing shared ownership of an object or array;

* [weak_ptr](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html#weak_ptr), a non-owning observer to a shared_ptr-managed object that can be promoted temporarily to shared_ptr;

* [intrusive_ptr](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html#intrusive_ptr), a pointer to objects with an embedded reference count;

* [local_shared_ptr](https://www.boost.org/doc/libs/1_74_0/libs/smart_ptr/doc/html/smart_ptr.html#local_shared_ptr), providing shared ownership within a single thread.

shared_ptr and weak_ptr are part of the C++ standard since its 2011 iteration.

scoped_ptr example:

```c++
#include <boost/scoped_ptr.hpp>
#include <iostream>

struct Shoe { ~Shoe() { std::cout << "Buckle my shoe\n"; } };

class MyClass {
    boost::scoped_ptr<int> ptr;
  public:
    MyClass() : ptr(new int) { *ptr = 0; }
    int add_one() { return ++*ptr; }
};

int main()
{
    boost::scoped_ptr<Shoe> x(new Shoe);
    MyClass my_instance;
    std::cout << my_instance.add_one() << '\n';
    std::cout << my_instance.add_one() << '\n';
}
```

Run it:

```c++
1
2
Buckle my shoe
```

[上一级](README.md)
[上一篇](algorithmSortNonStaticMemberFunction.md)
[下一篇](boostFileSystem.md)
