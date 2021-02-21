# c++ 内部类

```c++
#include <iostream>
using namespace std;

class outerB
{
public:
    outerB();
    ~outerB();
    void show();

    //嵌套类 innerA
    class innerA
    {
    public:
        innerA();
        ~innerA();
        void _show();
    };
private:
    innerA * m_p_innerA;
};



//outerB.cpp
outerB::outerB():m_p_innerA(new innerA)
{
    printf("%s \r\n",__FUNCTION__);
}

outerB::~outerB()
{
    printf("%s \r\n",__FUNCTION__);
    if (m_p_innerA!=NULL)
    {
        delete m_p_innerA;
    }
}


void outerB::show()
{
    m_p_innerA->_show();
    printf("%s \r\n",__FUNCTION__);
}
outerB::innerA::innerA()
{
    printf("%s \r\n",__FUNCTION__);
}
outerB::innerA::~innerA()
{
    printf("%s \r\n",__FUNCTION__);
}


void outerB::innerA::_show()
{
    printf("%s \r\n",__FUNCTION__);
}

int main()
{
    outerB x;
    x.show();
    return 0;
}

```

output:
```
innerA
outerB
_show
show
~outerB
~innerA
```
[上一级](README.md)
[上一篇 -> 当类中含有const，reference成员变量,其构造函数都需要初始化列表](initalization_list.md)
[下一篇 -> 链表](linkList.md)
