# 一些 Boost 程序库的简单使用

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [一些 Boost 程序库的简单使用](#一些-boost-程序库的简单使用)
  - [官网](#官网)
    - [windows 下安装 boost](#windows-下安装-boost)
  - [Any](#any)
  - [Format](#format)
  - [Lexical_Cast](#lexical_cast)
  - [Numeric Conversion](#numeric-conversion)
  - [Process](#process)
  - [Smart Ptr](#smart-ptr)
  - [Stacktrace](#stacktrace)
  - [thread](#thread)
  - [Property Tree](#property-tree)
  - [Asio](#asio)
    - [Basic Skills](#basic-skills)
      - [Using a timer synchronously](#using-a-timer-synchronously)
      - [Using a timer asynchronously:](#using-a-timer-asynchronously)
      - [Binding arguments to a handler:](#binding-arguments-to-a-handler)
      - [Using a member function as a handler:](#using-a-member-function-as-a-handler)
      - [Synchronising handlers in multithreaded programs:](#synchronising-handlers-in-multithreaded-programs)
    - [Introduction to Sockets](#introduction-to-sockets)
      - [A synchronous TCP daytime client:](#a-synchronous-tcp-daytime-client)
      - [A synchronous TCP daytime server:](#a-synchronous-tcp-daytime-server)
      - [An asynchronous TCP daytime server:](#an-asynchronous-tcp-daytime-server)
      - [A synchronous UDP daytime client:](#a-synchronous-udp-daytime-client)
      - [A synchronous UDP daytime server:](#a-synchronous-udp-daytime-server)
      - [An asynchronous UDP daytime server:](#an-asynchronous-udp-daytime-server)
      - [A combined TCP/UDP asynchronous server:](#a-combined-tcpudp-asynchronous-server)
      - [参考链接](#参考链接)
  - [Boost.Python](#boostpython)
    - [call your C++ code from Python](#call-your-c-code-from-python)
    - [call Python code from the C++-side](#call-python-code-from-the-c-side)

<!-- /code_chunk_output -->

## 官网

[Boost.org](https://www.boost.org/)

### windows 下安装 boost

1. 可直接下载[对应版本的源码](https://www.boost.org/users/download/)，来编译；
2. 直接下载 [Prebuilt windows binaries.](https://sourceforge.net/projects/boost/files/boost-binaries/)

参考链接：

* [How to use Boost in Visual Studio 2010](https://stackoverflow.com/questions/2629421/how-to-use-boost-in-visual-studio-2010)
* [在 windows 下安装 Boost 1.62.0](https://www.jianshu.com/p/004c99828af2)

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

[The Boost Numeric Conversion library](https://www.boost.org/doc/libs/1_74_0/libs/numeric/conversion/doc/html/index.html) is a collection of tools to describe and perform conversions between values of different numeric types.

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

[Boost.Process](https://www.boost.org/doc/libs/1_74_0/doc/html/process.html) is a library to manage system processes. It can be used to:

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

## Stacktrace

[Boost.Stacktrace](https://www.boost.org/doc/libs/1_74_0/doc/html/stacktrace.html) library is a simple C++03 library that provides information about call sequence in a human-readable form.

注：需要链接 dl 库；

print current call stack:

```c++
#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <boost/stacktrace.hpp>
#include <iostream>

int main()
{
    std::cout << boost::stacktrace::stacktrace();
}
```

Run it:

```sh
 0# boost::stacktrace::basic_stacktrace<std::allocator<boost::stacktrace::frame> >::basic_stacktrace() at /usr/include/boost/stacktrace/stacktrace.hpp:127
 1# __libc_start_main in /lib64/libc.so.6
 2# _start in /home/test/eclipse-workspace/boostTest/Debug/boostTest
```

Exceptions with stacktrace:

```c++
#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <boost/stacktrace.hpp>
#include <iostream>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

int main()
{
 try {
  throw boost::enable_error_info(std::out_of_range("throw exception"))
          << traced(boost::stacktrace::stacktrace());
 } catch (const std::exception& e) {
     std::cerr << e.what() << '\n';
     const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
     if (st) {
         std::cerr << *st << '\n';
     }
 }
}
```

Run it:

```sh
throw exception
 0# boost::stacktrace::basic_stacktrace<std::allocator<boost::stacktrace::frame> >::basic_stacktrace() at /usr/include/boost/stacktrace/stacktrace.hpp:127
 1# __libc_start_main in /lib64/libc.so.6
 2# _start in /home/test/eclipse-workspace/boostTest/Debug/boostTest
```

Better asserts:

```c++
#define BOOST_STACKTRACE_USE_ADDR2LINE
#define BOOST_ENABLE_ASSERT_DEBUG_HANDLER
// BOOST_ENABLE_ASSERT_DEBUG_HANDLER is defined for the whole project
#include <stdexcept>    // std::logic_error
#include <iostream>     // std::cerr
#include <boost/stacktrace.hpp>
#include <boost/assert.hpp>

namespace boost {
    inline void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const* /*file*/, long /*line*/) {
        std::cerr << "Expression '" << expr << "' is false in function '" << function << "': " << (msg ? msg : "<...>") << ".\n"
            << "Backtrace:\n" << boost::stacktrace::stacktrace() << '\n';

        std::abort();
    }

    inline void assertion_failed(char const* expr, char const* function, char const* file, long line) {
        ::boost::assertion_failed_msg(expr, 0 /*nullptr*/, function, file, line);
    }
} // namespace boost


int main(int argc, char **argv) {
 BOOST_ASSERT (0>1);
}
```

Run it:

```sh
Expression '0>1' is false in function 'int main(int, char**)': <...>.
Backtrace:
 0# boost::stacktrace::basic_stacktrace<std::allocator<boost::stacktrace::frame> >::basic_stacktrace() at /usr/include/boost/stacktrace/stacktrace.hpp:127 (discriminator 4)
 1# boost::assertion_failed(char const*, char const*, char const*, long) at /home/test/eclipse-workspace/boostTest/Debug/../main.cpp:19
 2# main at /home/test/eclipse-workspace/boostTest/Debug/../main.cpp:25
 3# __libc_start_main in /lib64/libc.so.6
 4# _start in /home/test/eclipse-workspace/boostTest/Debug/boostTest
```

* [boost::stacktrace::detail::location from symbol::location from symbol(void const*)': /usr/include/boost/stacktrace/detail/location_from_symbol.hpp:31: undefined reference to `dladdr'](https://github.com/boostorg/stacktrace/issues/80)
* [Boost stack-trace not showing function names and line numbers](https://stackoverflow.com/questions/52583544/boost-stack-trace-not-showing-function-names-and-line-numbers)

## thread

[Boost.Thread](https://www.boost.org/doc/libs/1_74_0/doc/html/thread.html) is accepted Thread C++11 library.

example:

```c++
#include <boost/thread.hpp>
#include <boost/thread/lock_guard.hpp>
#include <iostream>

class BankAccount {
    boost::mutex mtx_;
    int balance_;
public:
    void Deposit(int amount) {
        boost::lock_guard<boost::mutex> lg(mtx_);
        balance_ += amount;
    }
    void Withdraw(int amount) {
     boost::lock_guard<boost::mutex> lg(mtx_);
        balance_ -= amount;
    }
    int GetBalance() {
     boost::lock_guard<boost::mutex> lg(mtx_);
        return balance_;
    }
};

BankAccount g_joesAccount;

void bankAgent()
{
    for (int i =10; i>0; --i) {
        //...
        g_joesAccount.Deposit(500);
        //...
    }
}

void Joe() {
    for (int i =10; i>0; --i) {
        //...
        g_joesAccount.Withdraw(100);
        std::cout << g_joesAccount.GetBalance() << std::endl;
        //...
    }
}

int main() {
    //...
    boost::thread thread1(bankAgent); // start concurrent execution of bankAgent
    boost::thread thread2(Joe); // start concurrent execution of Joe
    thread1.join();
    thread2.join();
    return 0;
}
```

Run it:

```sh
4900
4800
4700
4600
4500
4400
4300
4200
4100
4000
```

* [Boost thread error: undefined reference](https://stackoverflow.com/questions/3584365/boost-thread-error-undefined-reference)

## Property Tree

[Boost.PropertyTree](https://www.boost.org/doc/libs/1_74_0/doc/html/property_tree.html)  provides a data structure that stores an arbitrarily deeply nested tree of values, indexed at each level by some key. Each node of the tree stores its own value, plus an ordered list of its subnodes and their keys. The tree allows easy access to any of its nodes by means of a path, which is a concatenation of multiple keys.

In addition, the library provides parsers and generators for a number of data formats that can be represented by such a tree, including XML, INI, and JSON.

xml example:
`debug_settings.xml`:

```xml
<debug>
<!-- debug -->
    <filename>debug.log</filename>
    <modules>
        <module>Finance</module>
        <module>Admin</module>
        <module>HR</module>
    </modules>
    <level>2</level>
    <Item name="project" desc="">
        <ChildItem name="project1" desc="file size" datatype="int">600</ChildItem>
        <ChildItem name="project2" desc="file size" datatype="int">353</ChildItem>
        <ChildItem name="project3" desc="file size" datatype="int">756</ChildItem>
        <ChildItem name="project4" desc="file size" datatype="int">888</ChildItem>
    </Item>
</debug>
```

```c++
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <map>
#include <boost/lexical_cast.hpp>
namespace pt = boost::property_tree;

typedef std::multimap<std::string,std::map<std::string,std::string>> AllChildAttrMap;

struct debug_settings
{
 std::string m_file;               // log filename
 int m_level;                      // debug level
 std::set<std::string> m_modules;  // modules where logging is enabled
 pt::ptree m_tree;
 AllChildAttrMap m_allChildItemAttrMap;
 std::map<std::string,std::string> m_itemAttr;
 void load(const std::string &filename);
 void save(const std::string &filename);
 void removeNode(const std::string& key,const std::string& val);
 void removeNodeAll(const std::string& nodePath);
 std::string getNodeAttr(const std::string& nodePath
  ,const std::string& attrName);
 std::map<std::string,std::string> getNodeAllAttr(
  const std::string& nodePath);
 void setNodeAttr(const std::string& nodePath,const std::string& attrName
  ,const std::string& attrVal);
 void setNodeAllAttr(const std::string& nodePath
  ,const std::map<std::string,std::string> allAttr);
 AllChildAttrMap getNodeAllChildAllAttr(
  const std::string& nodePath);
};

void debug_settings::load(const std::string &filename)
{

 // Parse the XML into the property tree.
 pt::read_xml(filename, m_tree
  ,boost::property_tree::xml_parser::trim_whitespace);

 // Use the throwing version of get to find the debug filename.
 // If the path cannot be resolved, an exception is thrown.
 m_file = m_tree.get<std::string>("debug.filename");

 // Use the default-value version of get to find the debug level.
 // Note that the default value is used to deduce the target type.
 // If the path cannot be resolved, Will not throw an exception
 m_level = m_tree.get("debug.level", 0);
 auto itemName = getNodeAttr("debug.Item","name");

 m_itemAttr = getNodeAllAttr("debug.Item");
 m_allChildItemAttrMap = getNodeAllChildAllAttr("debug.Item");

 // Use get_child to find the node containing the modules, and iterate over
 // its children. If the path cannot be resolved, get_child throws.
 // A C++11 for-range loop would also work.
 BOOST_FOREACH(pt::ptree::value_type &v, m_tree.get_child("debug.modules")) {
  // The data function is used to access the data stored in a node.
  m_modules.insert(v.second.data());
 }

}

void debug_settings::save(const std::string &filename)
{
 removeNode("debug.filename","notExists");
 // erase node debug.filename
 removeNode("debug","filename");
 m_tree.put("debug.filename",m_file);
 m_tree.put("debug.level", m_level);
 removeNodeAll("debug.modules");
 // Add all the modules. Unlike put, which overwrites existing nodes, add
 // adds a new node at the lowest level, so the "modules" node will have
 // multiple "module" children.
 BOOST_FOREACH(const std::string &name, m_modules)
  m_tree.add("debug.modules.module", name);

 removeNodeAll("debug.Item");
 setNodeAttr("debug.Item","test","ok");
 std::map<std::string,std::string> newChildItemAttr;
 newChildItemAttr.insert(std::make_pair("name","project1"));
 newChildItemAttr.insert(std::make_pair("desc","1"));
 setNodeAllAttr("debug.Item.newChildItem",newChildItemAttr);
 newChildItemAttr["name"]="project2";
 newChildItemAttr["desc"]="2";
 setNodeAllAttr("debug.Item.newChildItem1",newChildItemAttr);

 auto order = 0;
 for(auto it=m_allChildItemAttrMap.begin();it!=m_allChildItemAttrMap.end();++it){
  setNodeAllAttr("debug.Item."+it->first
   +boost::lexical_cast<std::string>(order++),it->second);
 }
 setNodeAttr("debug.Item","count"
  ,boost::lexical_cast<std::string>(6));
 // Better XML formatting
 boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);

 // Write property tree to XML file
 pt::write_xml(filename, m_tree,std::locale(),settings);
}



void debug_settings::removeNode(const std::string& key,const std::string& val)
{
 auto &children = m_tree.get_child(key);
 for(auto attrIt = children.begin(); attrIt != children.end();)
 {
  if(attrIt->first == val)
   attrIt = children.erase(attrIt);
  else
   ++attrIt;
 }
}

void debug_settings::removeNodeAll(const std::string& nodePath)
{
 auto &children = m_tree.get_child(nodePath);
 for(auto attrIt = children.begin(); attrIt != children.end();)
 {
  attrIt = children.erase(attrIt);
 }
 auto idx = nodePath.find_last_of(".");
 if(idx != std::string::npos){
  auto key = nodePath.substr(0,idx);
  auto value = nodePath.substr(idx+1);
  if(!key.empty() && ! value.empty()){
   removeNode(key,value);
  }
 }
}

std::string debug_settings::getNodeAttr(const std::string& nodePath ,const std::string& attrName)
{
 return  m_tree.get<std::string>(nodePath+".<xmlattr>."+attrName);
}

std::map<std::string,std::string> debug_settings::getNodeAllAttr(const std::string& nodePath)
{
 std::map<std::string,std::string> ret;
 BOOST_FOREACH(pt::ptree::value_type &v
  , m_tree.get_child(nodePath+".<xmlattr>")) {
   ret.insert(std::make_pair(v.first.data(),v.second.data()));
 }
 return ret;
}

void debug_settings::setNodeAttr(const std::string& nodePath
 ,const std::string& attrName ,const std::string& attrVal)
{
 m_tree.put(nodePath+".<xmlattr>."+attrName,attrVal);
}

void debug_settings::setNodeAllAttr(const std::string& nodePath ,const std::map<std::string,std::string> allAttr)
{
 for(auto it=allAttr.cbegin();it!=allAttr.cend();++it){
  m_tree.put(nodePath+".<xmlattr>."+it->first,it->second);
 }
}

AllChildAttrMap debug_settings::getNodeAllChildAllAttr(
 const std::string& nodePath)
{
 AllChildAttrMap ret;
 BOOST_FOREACH(pt::ptree::value_type &v, m_tree.get_child(nodePath)){
  BOOST_FOREACH(pt::ptree::value_type &v2, v.second){
   std::map<std::string,std::string> tmp;
   BOOST_FOREACH(pt::ptree::value_type &v3, v2.second){
    tmp.insert(std::make_pair(v3.first.data(),v3.second.data()));  
   }
   if(!tmp.empty()){
    ret.insert(std::make_pair(v.first,tmp));
   }
  }
  
 }
 return ret;
}

int main(int argc, char *argv[])
{
 try
 {
  debug_settings ds;
  ds.load("debug_settings.xml");
  ds.save("debug_settings_out.xml");
  std::cout << "Success\n";
 }
 catch (std::exception &e)
 {
  std::cout << "Error: " << e.what() << "\n";
 }
}
```

Run it, `debug_settings_out.xml`:

```xml
<?xml version="1.0" encoding="utf-8"?>
<debug>
<!-- debug -->
 <level>2</level>
 <filename>debug.log</filename>
 <modules>
  <module>Admin</module>
  <module>Finance</module>
  <module>HR</module>
 </modules>
 <Item test="ok" count="6">
  <newChildItem desc="1" name="project1"/>
  <newChildItem1 desc="2" name="project2"/>
  <ChildItem0 datatype="int" desc="file size" name="project1"/>
  <ChildItem1 datatype="int" desc="file size" name="project2"/>
  <ChildItem2 datatype="int" desc="file size" name="project3"/>
  <ChildItem3 datatype="int" desc="file size" name="project4"/>
 </Item>
</debug>
```

* [Better XML formatting using Boost? [duplicate]](https://stackoverflow.com/questions/27135813/better-xml-formatting-using-boost)
* [boost.property_tree的高级用法（你们没见过的操作）](https://blog.csdn.net/heshaai6843/article/details/80971375)
* [BOOST存储 XML格式化问题](https://blog.csdn.net/yulinxx/article/details/89915370)
* [How are attributes parsed in Boost.PropertyTree?](https://stackoverflow.com/questions/3690436/how-are-attributes-parsed-in-boost-propertytree)

## Asio

[Boost.Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html) is a cross-platform C++ library for network and low-level I/O programming that provides developers with a consistent asynchronous model using a modern C++ approach.

### Basic Skills

#### [Using a timer synchronously](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tuttimer1.html)

Print the `Hello, world!` message to show when the timer has expired(5s).

```c++
#include <iostream>
#include <boost/asio.hpp>

int main()
{
  boost::asio::io_context io;

  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

  t.wait();

  std::cout << "Hello, world!" << std::endl;

  return 0;
}
```

#### [Using a timer asynchronously](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tuttimer2.html):

```c++
#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

int main()
{
  boost::asio::io_context io;

  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
  t.async_wait(&print);

//The io_context::run() function will also continue
//to run while there is still "work" to do.
// In this example, the work is the asynchronous wait
//on the timer, so the call will not return until
//the timer has expired and the callback has completed.
  io.run();

  return 0;
}

```

#### [Binding arguments to a handler](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tuttimer3.html):

```c++
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

void print(const boost::system::error_code& /*e*/,
    boost::asio::steady_timer* t, int* count)
{
  if (*count < 5)
  {
    std::cout << *count << std::endl;
    ++(*count);

    //Next we move the expiry time for the timer along by /
    //one second from the previous expiry time. By
    //calculating the new expiry time relative to the old,
    //we can ensure that the timer does not drift away
    //from the whole-second mark due to any delays in
    //processing the handler.
    t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
    t->async_wait(boost::bind(print,
          boost::asio::placeholders::error, t, count));
  }
}

int main()
{
  boost::asio::io_context io;

  int count = 0;
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
  t.async_wait(boost::bind(print,
        boost::asio::placeholders::error, &t, &count));

  io.run();

  std::cout << "Final count is " << count << std::endl;

  return 0;
}
```

Run it:

```sh
0
1
2
3
4
Final count is 5
```

#### [Using a member function as a handler](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tuttimer4.html):

```c++
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class printer
{
public:
  printer(boost::asio::io_context& io)
    : timer_(io, boost::asio::chrono::seconds(1)),
      count_(0)
  {
    timer_.async_wait(boost::bind(&printer::print, this));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print()
  {
    if (count_ < 5)
    {
      std::cout << count_ << std::endl;
      ++count_;

      timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
      timer_.async_wait(boost::bind(&printer::print, this));
    }
  }

private:
  boost::asio::steady_timer timer_;
  int count_;
};

int main()
{
  boost::asio::io_context io;
  printer p(io);
  io.run();

  return 0;
}
```

#### [Synchronising handlers in multithreaded programs](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tuttimer5.html):

Consequently, calling io_context::run() from only one thread ensures that callback handlers cannot run concurrently.

if have a pool of threads calling io_context::run(). However, as this allows handlers to execute concurrently, we need a method of synchronisation when handlers might be accessing a shared, thread-unsafe resource.

use of the [strand](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/reference/strand.html) class template to synchronise callback handlers in a multithreaded program.

```c++
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>

class printer {
public:
 printer(boost::asio::io_context &io) :
   strand_(boost::asio::make_strand(io)), timer1_(io,
     boost::asio::chrono::seconds(1)), timer2_(io,
     boost::asio::chrono::seconds(1)), count_(0) {
  timer1_.async_wait(
    boost::asio::bind_executor(strand_,
      boost::bind(&printer::print1, this)));

  timer2_.async_wait(
    boost::asio::bind_executor(strand_,
      boost::bind(&printer::print2, this)));
 }

 ~printer() {
  std::cout << "Final count is " << count_ << std::endl;
 }

 void print1() {
  if (count_ < 10) {
   std::cout << "Timer 1: " << count_ << std::endl;
   ++count_;

   timer1_.expires_at(
     timer1_.expiry() + boost::asio::chrono::seconds(1));

   timer1_.async_wait(
     boost::asio::bind_executor(strand_,
       boost::bind(&printer::print1, this)));
  }
 }

 void print2() {
  if (count_ < 10) {
   std::cout << "Timer 2: " << count_ << std::endl;
   ++count_;

   timer2_.expires_at(
     timer2_.expiry() + boost::asio::chrono::seconds(1));

   timer2_.async_wait(
     boost::asio::bind_executor(strand_,
       boost::bind(&printer::print2, this)));
  }
 }

private:
 boost::asio::strand<boost::asio::io_context::executor_type> strand_;
 boost::asio::steady_timer timer1_;
 boost::asio::steady_timer timer2_;
 int count_;
};

int main() {
 //The main function now causes io_context::run() to be called from two threads:
 //the main thread and one additional thread.
 //This is accomplished using an boost::thread object.
 boost::asio::io_context io;
 printer p(io);
 boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
 //Just as it would with a call from a single thread,
 //concurrent calls to io_context::run() will continue to
 //execute while there is "work" left to do. The background
 //thread will not exit until all asynchronous operations have completed.
 io.run();
 t.join();

 return 0;
}
```

Run it:

```sh
Timer 1: 0
Timer 2: 1
Timer 1: 2
Timer 2: 3
Timer 1: 4
Timer 2: 5
Timer 1: 6
Timer 2: 7
Timer 1: 8
Timer 2: 9
Final count is 10
```

### Introduction to Sockets

#### [A synchronous TCP daytime client](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime1.html):

```c++
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    //A resolver takes a host name and service name and turns them into a list of endpoints.
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime");

    //Now we create and connect the socket. The list of endpoints obtained
    //above may contain both IPv4 and IPv6 endpoints
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
```

#### [A synchronous TCP daytime server](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime2.html):

```c++
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
 using namespace std;
 // For time_t, time and ctime;
 time_t now = time(0);
 return ctime(&now);
}

int main() {
 try {
  boost::asio::io_context io_context;

  //A ip::tcp::acceptor object needs to be created to
  // listen for new connections. It is initialised to
  //listen on TCP port 13, for IP version 4.
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

  for (;;) {
   tcp::socket socket(io_context);
   acceptor.accept(socket);

   std::string message = make_daytime_string();

   boost::system::error_code ignored_error;
   boost::asio::write(socket, boost::asio::buffer(message),
     ignored_error);
  }
 } catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
 }

 return 0;
}
```

#### [An asynchronous TCP daytime server](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime3.html):

```c++
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
 using namespace std;
 // For time_t, time and ctime;
 time_t now = time(0);
 return ctime(&now);
}

//We will use shared_ptr and enable_shared_from_this because
//we want to keep the tcp_connection object alive as long as
//there is an operation that refers to it.
class tcp_connection: public boost::enable_shared_from_this<tcp_connection> {
public:
 typedef boost::shared_ptr<tcp_connection> pointer;

 static pointer create(boost::asio::io_context &io_context) {
  return pointer(new tcp_connection(io_context));
 }

 tcp::socket& socket() {
  return socket_;
 }

 void start() {
  message_ = make_daytime_string();

  boost::asio::async_write(socket_, boost::asio::buffer(message_),
    boost::bind(&tcp_connection::handle_write, shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
 }

private:
 tcp_connection(boost::asio::io_context &io_context) :
   socket_(io_context) {
 }

 void handle_write(const boost::system::error_code& /*error*/,
   size_t /*bytes_transferred*/) {
 }

 tcp::socket socket_;
 std::string message_;
};

class tcp_server {
public:
 tcp_server(boost::asio::io_context &io_context) :
   io_context_(io_context), acceptor_(io_context,
     tcp::endpoint(tcp::v4(), 13)) {
  start_accept();
 }

private:
 //creates a socket and initiates an asynchronous accept
 //operation to wait for a new connection.
 void start_accept() {
  tcp_connection::pointer new_connection = tcp_connection::create(
    io_context_);

  acceptor_.async_accept(new_connection->socket(),
    boost::bind(&tcp_server::handle_accept, this, new_connection,
      boost::asio::placeholders::error));
 }

 //The function handle_accept() is called
 //when the asynchronous
 //accept operation initiated by start_accept() finishes.
 //It services the client request, and then calls
 //start_accept() to initiate the next accept operation.
 void handle_accept(tcp_connection::pointer new_connection,
   const boost::system::error_code &error) {
  if (!error) {
   new_connection->start();
  }

  start_accept();
 }

 boost::asio::io_context &io_context_;
 tcp::acceptor acceptor_;
};

int main() {
 try {
  boost::asio::io_context io_context;
  tcp_server server(io_context);
  //Run the io_context object so that
  //it will perform asynchronous
  //operations on your behalf.
  io_context.run();
 } catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
 }

 return 0;
}
```

#### [A synchronous UDP daytime client](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime4.html):

```c++
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char *argv[]) {
 try {
  if (argc != 2) {
   std::cerr << "Usage: client <host>" << std::endl;
   return 1;
  }

  boost::asio::io_context io_context;

  udp::resolver resolver(io_context);
  udp::endpoint receiver_endpoint =
    *resolver.resolve(udp::v4(), argv[1],
    "daytime").begin();

  udp::socket socket(io_context);
  socket.open(udp::v4());

  boost::array<char, 1> send_buf = { { 0 } };
  socket.send_to(boost::asio::buffer(send_buf)
    , receiver_endpoint);

  boost::array<char, 128> recv_buf;
  udp::endpoint sender_endpoint;
  size_t len = socket.receive_from(
    boost::asio::buffer(recv_buf),
    sender_endpoint);

  std::cout.write(recv_buf.data(), len);
 } catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
 }

 return 0;
}
```

#### [A synchronous UDP daytime server](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime5.html):

```c++
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
 using namespace std;
 // For time_t, time and ctime;
 time_t now = time(0);
 return ctime(&now);
}

int main() {
 try {
  boost::asio::io_context io_context;

  udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

  for (;;) {
   boost::array<char, 1> recv_buf;
   udp::endpoint remote_endpoint;
   socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

   std::string message = make_daytime_string();

   boost::system::error_code ignored_error;
   socket.send_to(boost::asio::buffer(message), remote_endpoint, 0,
     ignored_error);
  }
 } catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
 }

 return 0;
}
```

#### [An asynchronous UDP daytime server](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime6.html):

```c++
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
 using namespace std;
 // For time_t, time and ctime;
 time_t now = time(0);
 return ctime(&now);
}

class udp_server {
public:
 //initialises a socket to listen on UDP port 13.
 udp_server(boost::asio::io_context &io_context) :
   socket_(io_context, udp::endpoint(udp::v4(), 13)) {
  start_receive();
 }

private:
 void start_receive() {
  //The function ip::udp::socket::async_receive_from() will
  //cause the application to listen in the background for a new request.
  //When such a request is received, the io_context object
  //will invoke the handle_receive() function
  socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
    remote_endpoint_,
    boost::bind(&udp_server::handle_receive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
 }

 void handle_receive(const boost::system::error_code &error,
   std::size_t /*bytes_transferred*/) {
  //The error parameter contains the result of the asynchronous operation
  if (!error) {
   boost::shared_ptr<std::string> message(
     new std::string(make_daytime_string()));

   //serve the data to the client.
   socket_.async_send_to(boost::asio::buffer(*message),
     remote_endpoint_,
     boost::bind(&udp_server::handle_send, this, message,
       boost::asio::placeholders::error,
       boost::asio::placeholders::bytes_transferred));

   start_receive();
  }
 }

 //The function is invoked after
 //the service request has been completed.
 void handle_send(boost::shared_ptr<std::string> /*message*/,
   const boost::system::error_code& /*error*/,
   std::size_t /*bytes_transferred*/) {
 }

 udp::socket socket_;
 udp::endpoint remote_endpoint_;
 boost::array<char, 1> recv_buffer_;
};

int main() {
 try {
  boost::asio::io_context io_context;
  //Create a server object to accept incoming client requests
  //, and run the io_context object.
  udp_server server(io_context);
  io_context.run();
 } catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
 }

 return 0;
}
```

#### [A combined TCP/UDP asynchronous server](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime7.html):

```c++
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

std::string make_daytime_string() {
 using namespace std;
 // For time_t, time and ctime;
 time_t now = time(0);
 return ctime(&now);
}

class tcp_connection: public boost::enable_shared_from_this<tcp_connection> {
public:
 typedef boost::shared_ptr<tcp_connection> pointer;

 static pointer create(boost::asio::io_context &io_context) {
  return pointer(new tcp_connection(io_context));
 }

 tcp::socket& socket() {
  return socket_;
 }

 void start() {
  message_ = make_daytime_string();

  boost::asio::async_write(socket_, boost::asio::buffer(message_),
    boost::bind(&tcp_connection::handle_write, shared_from_this()));
 }

private:
 tcp_connection(boost::asio::io_context &io_context) :
   socket_(io_context) {
 }

 void handle_write() {
 }

 tcp::socket socket_;
 std::string message_;
};

class tcp_server {
public:
 tcp_server(boost::asio::io_context &io_context) :
   io_context_(io_context), acceptor_(io_context,
     tcp::endpoint(tcp::v4(), 13)) {
  start_accept();
 }

private:
 void start_accept() {
  tcp_connection::pointer new_connection = tcp_connection::create(
    io_context_);

  acceptor_.async_accept(new_connection->socket(),
    boost::bind(&tcp_server::handle_accept, this, new_connection,
      boost::asio::placeholders::error));
 }

 void handle_accept(tcp_connection::pointer new_connection,
   const boost::system::error_code &error) {
  if (!error) {
   new_connection->start();
  }

  start_accept();
 }

 boost::asio::io_context &io_context_;
 tcp::acceptor acceptor_;
};

class udp_server {
public:
 udp_server(boost::asio::io_context &io_context) :
   socket_(io_context, udp::endpoint(udp::v4(), 13)) {
  start_receive();
 }

private:
 void start_receive() {
  socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
    remote_endpoint_,
    boost::bind(&udp_server::handle_receive, this,
      boost::asio::placeholders::error));
 }

 void handle_receive(const boost::system::error_code &error) {
  if (!error) {
   boost::shared_ptr<std::string> message(
     new std::string(make_daytime_string()));

   socket_.async_send_to(boost::asio::buffer(*message),
     remote_endpoint_,
     boost::bind(&udp_server::handle_send, this, message));

   start_receive();
  }
 }

 void handle_send(boost::shared_ptr<std::string> /*message*/) {
 }

 udp::socket socket_;
 udp::endpoint remote_endpoint_;
 boost::array<char, 1> recv_buffer_;
};

int main() {
 try {
  boost::asio::io_context io_context;
  tcp_server server1(io_context);
  udp_server server2(io_context);
  io_context.run();
 } catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
 }

 return 0;
}
```

#### 参考链接
* [Get Local IP-Address using Boost.Asio](https://stackoverflow.com/questions/2674314/get-local-ip-address-using-boost-asio)
* [Boost asio socket: how to get IP, port address of connection?](https://stackoverflow.com/questions/5352757/boost-asio-socket-how-to-get-ip-port-address-of-connection/5352979)
* [Configuring TCP keep_alive with boost::asio [duplicate]](https://stackoverflow.com/questions/20188718/configuring-tcp-keep-alive-with-boostasio)



## Boost.Python

[Boost.Python](https://www.boost.org/doc/libs/1_74_0/libs/python/doc/html/index.html) is a C++ library which enables seamless interoperability between C++ and the Python programming language.
The library includes support for:

* References and Pointers
* Globally Registered Type Coercions
* Automatic Cross-Module Type Conversions
* Efficient Function Overloading
* C++ to Python Exception Translation
* Default Arguments
* Keyword Arguments
* Manipulating Python objects in C++
* Exporting C++ Iterators as Python Iterators
* Documentation Strings

### call your C++ code from Python

[Boost.Python Tutorial](https://www.boost.org/doc/libs/1_74_0/libs/python/doc/html/tutorial/index.html)

### call Python code from the C++-side

[Embedding](https://www.boost.org/doc/libs/1_74_0/libs/python/doc/html/tutorial/tutorial/embedding.html)

[上一级](README.md)
[上一篇](algorithmSortNonStaticMemberFunction.md)
[下一篇](boostFileSystem.md)
