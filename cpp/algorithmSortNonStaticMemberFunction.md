# problem sorting using member function as comparator



<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [problem sorting using member function as comparator](#problem-sorting-using-member-function-as-comparator)
	* [doCompare must be static](#docompare-must-be-static)
	* [std::sort with a non-static compare method](#stdsort-with-a-non-static-compare-method)
	* [Link](#link)

<!-- /code_chunk_output -->

## doCompare must be static
```c++
#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;
struct FileValue {
  int temperature;
  int frequency;
  int channel;
  FileValue(int t, int f, int c) : temperature(t), frequency(f), channel(c) {}
};

struct FileStruct {
  std::string fileName;
  FileValue fileValue;
};
class FileNameSort {
private:
  std::vector<FileValue> m_str;

public:
  FileNameSort() {
    FileValue a(1, 5, 1);
    FileValue b(2, 4, 2);
    FileValue c(3, 3, 1);
    FileValue d(4, 2, 2);
    FileValue e(5, 1, 1);
    m_str.push_back(a);
    m_str.push_back(b);
    m_str.push_back(c);
    m_str.push_back(d);
    m_str.push_back(e);
  }
  bool Compare3Value(FileValue &a, FileValue &b) {
    if (a.channel > b.channel)
      return false;
    else if (a.channel == b.channel) {
      if (a.frequency > b.frequency)
        return false;
      else if (a.frequency == b.frequency) {
        if (a.frequency > b.frequency)
          return false;
      }
    }
    return true;
  }
  void handle() {
    std::sort(m_str.begin(), m_str.end(), Compare3Value);
    for (auto i : m_str) {
      std::cout << "temperature:" << i.temperature
                << ",frequency:" << i.frequency << ",channel:" << i.channel
                << std::endl;
    }
  }
  virtual ~FileNameSort() {}
};

bool compareNat(const std::string &a, const std::string &b) {
  if (a.empty())
    return true;
  if (b.empty())
    return false;
  if (std::isdigit(a[0]) && !std::isdigit(b[0]))
    return true;
  if (!std::isdigit(a[0]) && std::isdigit(b[0]))
    return false;
  if (!std::isdigit(a[0]) && !std::isdigit(b[0])) {
    if (std::toupper(a[0]) == std::toupper(b[0]))
      return compareNat(a.substr(1), b.substr(1));
    return (std::toupper(a[0]) < std::toupper(b[0]));
  }
  // Both strings begin with digit --> parse both numbers
  std::istringstream issa(a);
  std::istringstream issb(b);
  int ia, ib;
  issa >> ia;
  issb >> ib;
  if (ia != ib)
    return ia < ib;

  // Numbers are the same -->remove numbers and recurse
  std::string anew, bnew;
  std::getline(issa, anew);
  std::getline(issb, bnew);
  return (compareNat(anew, bnew));
}

void ParseFileName(std::string s) {
  std::smatch m;
  std::regex e("\\d+");

  while (std::regex_search(s, m, e)) {
    for (auto x : m)
      std::cout << x << " ";
    std::cout << std::endl;
    s = m.suffix().str();
  }
}

int main() {
  // ParseFileName("Data20_30_CH1");
  FileNameSort test;
  test.handle();
}
```

在fedora 28下编译：
```sh
sortFileName.cpp: In member function ‘void FileNameSort::handle()’:
sortFileName.cpp:52:56: error: invalid use of non-static member function ‘bool FileNameSort::Compare3Value(FileValue&, FileValue&)’
     std::sort(m_str.begin(), m_str.end(), Compare3Value);
                                                        ^
sortFileName.cpp:38:8: note: declared here
   bool Compare3Value(FileValue &a, FileValue &b) {
        ^~~~~~~~~~~~~
```

在VS2015下编译：
```sh
1>e:\customworkspace\project\sortfilename\sortfilename\main.cpp(53): error C3867: “FileNameSort::Compare3Value”: 非标准语法；请使用 "&" 来创建指向成员的指针
1>e:\customworkspace\project\sortfilename\sortfilename\main.cpp(53): error C2672: “std::sort”: 未找到匹配的重载函数
1>e:\customworkspace\project\sortfilename\sortfilename\main.cpp(53): error C2780: “void std::sort(_RanIt,_RanIt)”: 应输入 2 个参数，却提供了 3 个
1>  e:\program files (x86)\microsoft visual studio 14.0\vc\include\algorithm(2789): note: 参见“std::sort”的声明
```

解决办法：
```c++
bool static Compare3Value(FileValue &a, FileValue &b);
```

## std::sort with a non-static compare method

```c++
#include <vector>
#include <algorithm>
#include <iostream>

using namespace::std;

class myclass
{
	bool sortascending;	// this is the data our sort depends on
	vector<int> vec;

	struct sortstruct
	{
		// sortstruct needs to know its containing object
		myclass* m;
		sortstruct(myclass* p) : m(p) {};

		// this is our sort function, which makes use
		// of some non-static data (sortascending)
		bool operator() ( int i, int j )
		{
			if ( m->sortascending )
				return i < j;
			else return i > j;
		}
	};

public:
	myclass ( bool b ) : sortascending(b) {}

	void init  ()
	{	// just some values for testing
		vec.push_back(3); vec.push_back(100);
		vec.push_back(10); vec.push_back(143);
	}
	void sort  ()
	{
		// create a sortstruct and pass it to std::sort
		sortstruct s(this);
		std::sort ( vec.begin (), vec.end (), s );
	}
	void print ()
	{	// print the vector values to the console
		for (int i = 0; i < vec.size(); i++)
			std::cout << vec[i] << std::endl;
	}
};

int main()
{
	myclass m(true);
	m.init();
	m.sort();
	m.print();
}
```

## Link
* [problem sorting using member function as comparator
](https://stackoverflow.com/questions/1902311/problem-sorting-using-member-function-as-comparator?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)
* [std::sort with a non-static compare method](http://tipsandtricks.runicsoft.com/Cpp/Sort.html)

* [Why callback functions needs to be static when declared in class
](https://stackoverflow.com/questions/2400690/why-callback-functions-needs-to-be-static-when-declared-in-class?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)

---
- [上一级](README.md)
- 上一篇 -> [STL](STL.md)
- 下一篇 -> [一些 Boost 程序库的简单使用](boost.md)
