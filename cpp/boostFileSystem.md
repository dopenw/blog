# Boost file system

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Boost file system](#boost-file-system)
	* [Sample 1: 简单使用](#sample-1-简单使用)
		* [编译生成可执行文件](#编译生成可执行文件)
	* [Sample 2：使用 directory_iterator输出目录下的(文件和目录)](#sample-2使用-directory_iterator输出目录下的文件和目录)
		* [使用 recursive_directory_iterator 递归输出](#使用-recursive_directory_iterator-递归输出)
	* [Sample 3：排序目录里面的(文件和目录)](#sample-3排序目录里面的文件和目录)
	* [Sample 4: path 类的一些方法](#sample-4-path-类的一些方法)
	* [Sample 5:create directory](#sample-5create-directory)
	* [在 IDE 中使用 boost 库](#在-ide-中使用-boost-库)
	* [Link](#link)

<!-- /code_chunk_output -->

## Sample 1: 简单使用
```c++
#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut2 path\n";
    return 1;
  }

  path p(argv[1]);  // avoid repeated path construction below

  if (exists(p))    // does path p actually exist?
  {
    if (is_regular_file(p))        // is path p a regular file?
      cout << p << " size is " << file_size(p) << '\n';

    else if (is_directory(p))      // is path p a directory?
      cout << p << " is a directory\n";

    else
      cout << p << " exists, but is not a regular file or directory\n";
  }
  else
    cout << p << " does not exist\n";

  return 0;
}
```

### 编译生成可执行文件

```c++
g++ -lboost_filesystem -lboost_system main.cpp -o out
```

## Sample 2：使用 directory_iterator输出目录下的(文件和目录)

Boost.Filesystem's directory_iterator class is just what we need here. It follows the general pattern of the standard library's istream_iterator. Constructed from a path, it iterates over the contents of the directory. A default constructed directory_iterator acts as the end iterator.

The value type of directory_iterator is directory_entry. A directory_entry object contains path and file_status information.  A directory_entry object can be used directly, but can also be passed to path arguments in function calls.

如果路径是目录，就输出该路径下的文件和目录（不是递归输出！）
```c++
#include <iostream>
#include <boost/filesystem.hpp>
using std::cout;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut3 path\n";
    return 1;
  }

  path p (argv[1]);

  try
  {
    if (exists(p))
    {
      if (is_regular_file(p))
        cout << p << " size is " << file_size(p) << '\n';

      else if (is_directory(p))
      {
        cout << p << " is a directory containing:\n";

        for (directory_entry& x : directory_iterator(p))
          cout << "    " << x.path() << '\n';
      }
      else
        cout << p << " exists, but is not a regular file or directory\n";
    }
    else
      cout << p << " does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return 0;
}
```
Output:
```sh
[breap@localhost Debug]$ ./boostFileSystem ~/workspace/boostFileSystem/
"/home/breap/workspace/boostFileSystem/" is a directory containing:
    "/home/breap/workspace/boostFileSystem/.cproject"
    "/home/breap/workspace/boostFileSystem/Debug"
    "/home/breap/workspace/boostFileSystem/main.cpp"
    "/home/breap/workspace/boostFileSystem/.project"
    "/home/breap/workspace/boostFileSystem/.settings"
[breap@localhost Debug]$ ./boostFileSystem /root
"/root" is a directory containing:
boost::filesystem::directory_iterator::construct: Permission denied: "/root"
```

### 使用 recursive_directory_iterator 递归输出
```c++
#include <iostream>
#include <boost/filesystem.hpp>
using std::cout;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut3 path\n";
    return 1;
  }

  path p (argv[1]);

  try
  {
    if (exists(p))
    {
      if (is_regular_file(p))
        cout << p << " size is " << file_size(p) << '\n';

      else if (is_directory(p))
      {
        cout << p << " is a directory containing:\n";

        for (directory_entry& x : recursive_directory_iterator(p))
          cout << "    " << x.path() << '\n';
      }
      else
        cout << p << " exists, but is not a regular file or directory\n";
    }
    else
      cout << p << " does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return 0;
}
```



## Sample 3：排序目录里面的(文件和目录)

排序输出：
```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
using std::cout;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut4 path\n";
    return 1;
  }

  path p (argv[1]);

  try
  {
    if (exists(p))
    {
      if (is_regular_file(p))
        cout << p << " size is " << file_size(p) << '\n';

      else if (is_directory(p))
      {
        cout << p << " is a directory containing:\n";

        std::vector<path> v;

        for (auto&& x : directory_iterator(p))
          v.push_back(x.path());

        std::sort(v.begin(), v.end());  

        for (auto&& x : v)
          cout << "    " << x.filename() << '\n';
      }
      else
        cout << p << " exists, but is not a regular file or directory\n";
    }
    else
      cout << p << " does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return 0;
}
```

## Sample 4: path 类的一些方法
```c++
#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

const char * say_what(bool b) { return b ? "true" : "false"; }

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: path_info path-element [path-element...]\n"
            "Composes a path via operator/= from one or more path-element arguments\n"
            "Example: path_info foo/bar baz\n"
#            ifdef BOOST_POSIX_API
            "         would report info about the composed path foo/bar/baz\n";
#            else  // BOOST_WINDOWS_API
            "         would report info about the composed path foo/bar\\baz\n";
#            endif
    return 1;
  }

  path p;
  for (; argc > 1; --argc, ++argv)
    p /= argv[1];  // compose path p from the command line arguments

  cout  <<  "\ncomposed path:\n";
  cout  <<  "  operator<<()---------: " << p << "\n";
  cout  <<  "  make_preferred()-----: " << p.make_preferred() << "\n";

  cout << "\nelements:\n";
  for (auto element : p)
    cout << "  " << element << '\n';

  cout  <<  "\nobservers, native format:" << endl;
# ifdef BOOST_POSIX_API
  cout  <<  "  native()-------------: " << p.native() << endl;
  cout  <<  "  c_str()--------------: " << p.c_str() << endl;
# else  // BOOST_WINDOWS_API
  wcout << L"  native()-------------: " << p.native() << endl;
  wcout << L"  c_str()--------------: " << p.c_str() << endl;
# endif
  cout  <<  "  string()-------------: " << p.string() << endl;
  wcout << L"  wstring()------------: " << p.wstring() << endl;

  cout  <<  "\nobservers, generic format:\n";
  cout  <<  "  generic_string()-----: " << p.generic_string() << endl;
  wcout << L"  generic_wstring()----: " << p.generic_wstring() << endl;

  cout  <<  "\ndecomposition:\n";
  cout  <<  "  root_name()----------: " << p.root_name() << '\n';
  cout  <<  "  root_directory()-----: " << p.root_directory() << '\n';
  cout  <<  "  root_path()----------: " << p.root_path() << '\n';
  cout  <<  "  relative_path()------: " << p.relative_path() << '\n';
  cout  <<  "  parent_path()--------: " << p.parent_path() << '\n';
  cout  <<  "  filename()-----------: " << p.filename() << '\n';
  cout  <<  "  stem()---------------: " << p.stem() << '\n';
  cout  <<  "  extension()----------: " << p.extension() << '\n';

  cout  <<  "\nquery:\n";
  cout  <<  "  empty()--------------: " << say_what(p.empty()) << '\n';
  cout  <<  "  is_absolute()--------: " << say_what(p.is_absolute()) << '\n';
  cout  <<  "  has_root_name()------: " << say_what(p.has_root_name()) << '\n';
  cout  <<  "  has_root_directory()-: " << say_what(p.has_root_directory()) << '\n';
  cout  <<  "  has_root_path()------: " << say_what(p.has_root_path()) << '\n';
  cout  <<  "  has_relative_path()--: " << say_what(p.has_relative_path()) << '\n';
  cout  <<  "  has_parent_path()----: " << say_what(p.has_parent_path()) << '\n';
  cout  <<  "  has_filename()-------: " << say_what(p.has_filename()) << '\n';
  cout  <<  "  has_stem()-----------: " << say_what(p.has_stem()) << '\n';
  cout  <<  "  has_extension()------: " << say_what(p.has_extension()) << '\n';

  return 0;
}
```

Output:
```sh
[breap@localhost Debug]$ ./boostFileSystem /tmp

composed path:
  operator<<()---------: "/tmp"
  make_preferred()-----: "/tmp"

elements:
  "/"
  "tmp"

observers, native format:
  native()-------------: /tmp
  c_str()--------------: /tmp
  string()-------------: /tmp
  wstring()------------: /tmp

observers, generic format:
  generic_string()-----: /tmp
  generic_wstring()----: /tmp

decomposition:
  root_name()----------: ""
  root_directory()-----: "/"
  root_path()----------: "/"
  relative_path()------: "tmp"
  parent_path()--------: "/"
  filename()-----------: "tmp"
  stem()---------------: "tmp"
  extension()----------: ""

query:
  empty()--------------: false
  is_absolute()--------: true
  has_root_name()------: false
  has_root_directory()-: true
  has_root_path()------: true
  has_relative_path()--: true
  has_parent_path()----: true
  has_filename()-------: true
  has_stem()-----------: true
  has_extension()------: false
```

## Sample 5:create directory
```c++
#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost::filesystem;

int main()
{
  path p{"./Test"};
  try
  {
    if (create_directory(p))
    {
      rename(p, "./Test2");
      std::cout<<"mv Test Test2"<<std::endl;
     // boost::filesystem::remove("./Test2");
    }
  }
  catch (filesystem_error &e)
  {
    std::cerr << e.what() << '\n';
  }
}
```

Output:
```sh
[breap@localhost Debug]$ ./boostFileSystem
mv Test Test2
```

[The boost filesystem :Files and Directories](https://theboostcpplibraries.com/boost.filesystem-files-and-directories)

## 在 IDE 中使用 boost 库

[Boost with Qt Creator and Linux](https://stackoverflow.com/questions/13480765/boost-with-qt-creator-and-linux)

[visual studio use boost](https://stackoverflow.com/questions/13042561/fatal-error-lnk1104-cannot-open-file-libboost-system-vc110-mt-gd-1-51-lib)

## Link
* [Boost Filesystem Library Version 3](https://www.boost.org/doc/libs/1_67_0/libs/filesystem/doc/index.htm)
* [Boost Filesystem Tutorial](https://www.boost.org/doc/libs/1_67_0/libs/filesystem/doc/tutorial.html)
* [Can't link program using Boost.Filesystem](https://stackoverflow.com/questions/15634114/cant-link-program-using-boost-filesystem)
* [The boost filesystem :Files and Directories](https://theboostcpplibraries.com/boost.filesystem-files-and-directories)

[上一级](base.md)
[上一篇](algorithmSortNonStaticMemberFunction.md)
[下一篇](chrono.md)
