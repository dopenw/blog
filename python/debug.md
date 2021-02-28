# 调试

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [调试](#调试)
  - [抛出异常](#抛出异常)
  - [取得反向跟踪的字符串](#取得反向跟踪的字符串)
  - [断言](#断言)
    - [禁用断言](#禁用断言)
  - [日志](#日志)
    - [使用日志模块](#使用日志模块)
    - [日志等级](#日志等级)
    - [禁用日志](#禁用日志)
    - [将日志记录到文件](#将日志记录到文件)

<!-- /code_chunk_output -->

注：《Python 编程快速上手-让繁琐工作自动化》 第 10 章调试学习笔记。

## 抛出异常

当 Python 试图执行无效代码时，就会抛出异常。如果需要抛出异常则可使用 raise 语句。使用 try 和 except 语句来处理 Python 的异常。eg：
```python
>>> raise Exception('This is the error message.')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
Exception: This is the error message.
>>> 
```

## 取得反向跟踪的字符串
如果 python 遇到错误，它就会生成一些错误信息，称为 "反向跟踪"。反向跟踪包含了出错信息、导致该错误的代码行号，以及导致该错误的函数调用的序列。这个序列称为“调用栈”。

```python
def spam():
  bacon()
def bacon():
  raise Exception('This is the error message')

spam()
```
Run it,show:
```python
Traceback (most recent call last):
  File "tmp.py", line 6, in <module>
    spam()
  File "tmp.py", line 2, in spam
    bacon()
  File "tmp.py", line 4, in bacon
    raise Exception('This is the error message')
Exception: This is the error message
```
通过反向跟踪，我们可以的之错误发生在什么地方。

只要抛出的异常没有被处理，Python 就会显示反向跟踪。但你也可以调用 [traceback](https://docs.python.org/3/library/traceback.html).format_exc()，得到它的字符串形式。

示例：
```c++
import traceback

try:
    raise Exception('This is the error message.')
except:
    errorFile = open('errorInfo.txt','w')
    errorFile.write(traceback.format_exc())
    traceback.print_exc()
    errorFile.close()
    print('The traceback info was written to errorInfo.txt.')
```
Run it,show:
```python
Traceback (most recent call last):
  File "tmp.py", line 4, in <module>
    raise Exception('This is the error message.')
Exception: This is the error message.
The traceback info was written to errorInfo.txt.
```
errorInfo.txt 内容：
```python
Traceback (most recent call last):
  File "tmp.py", line 4, in <module>
    raise Exception('This is the error message.')
Exception: This is the error message.
```

## 断言
[Python assert Keyword](https://www.w3schools.com/python/ref_keyword_assert.asp)

### 禁用断言
在运行 Python 时传入 -O 选项，可以禁用断言。

## 日志
如果你曾经在代码中加入 print() 语句，在程序运行时输出某些变量的值，你就使用了记日志的方式来调试代码。记日志是一种很好的方式，可以理解程序中发生的事，以及事件发生的顺序。python 的 [logging](https://docs.python.org/3/howto/logging.html)模块使得你很容易创建自定义的消息记录。

### 使用日志模块
要启用 logging 模块，在程序运行时将日志信息显示在屏幕上，请将下面的代码复制到程序顶部（但在 Python 的 #!之下）:
```python
import logging
logging.basicConfig(level = logging.DEBUG,format \
  = ' %(asctime)s - %(levelname)s - %(message)s')
```
示例：
```python
import logging
logging.basicConfig(level = logging.DEBUG,format \
  = ' %(asctime)s - %(levelname)s - %(message)s')

logging.debug('Start of program')

def factorial(n):
    logging.debug('Start of factorial({})'.format(n))
    total = 1
    for i in range(n+1):
        total *= i
        logging.debug('i is {} ,total is {}'.format(i,total))

    logging.debug('End of factorial({})'.format(n))
    return total

print(' factorial(5) result is {}'.format(factorial(5)))
logging.debug('End of program')
```
Run it,show:
```log
 2021-02-28 12:29:16,515 - DEBUG - Start of program
 2021-02-28 12:29:16,515 - DEBUG - Start of factorial(5)
 2021-02-28 12:29:16,515 - DEBUG - i is 0 ,total is 0
 2021-02-28 12:29:16,515 - DEBUG - i is 1 ,total is 0
 2021-02-28 12:29:16,515 - DEBUG - i is 2 ,total is 0
 2021-02-28 12:29:16,515 - DEBUG - i is 3 ,total is 0
 2021-02-28 12:29:16,515 - DEBUG - i is 4 ,total is 0
 2021-02-28 12:29:16,515 - DEBUG - i is 5 ,total is 0
 2021-02-28 12:29:16,515 - DEBUG - End of factorial(5)
 factorial(5) result is 0
 2021-02-28 12:29:16,515 - DEBUG - End of program
```

### 日志等级
“日志级别”提供了一种方式，按重要性对日志信息进行分类。级别如下所示：
|  级别 |  日志函数 	|   描述	|
|:-:	|---	|---	|
|  DEBUG 	|  logging.debug() 	|  最低级别。用于小细节。通常只有在诊断问题时，你才会关心这些消息 	|
|  INFO 	|   logging.info()	|   用于记录程序中一般事件信息，或确认一切工作正常	|
|  WARNING 	|  logging.warning() 	|  用于描述可能的问题，它不会阻止程序的工作，但将来可能会 	|
|   ERROR	|  logging.error() 	|  用于记录错误，它导致程序做某事失败 	|
|   CRITICAL	|  logging.critical() 	|  最高级别。用于表示致命错误，它导致或将要导致程序完全停止 |

### 禁用日志 
只要向 logging.disable() 传入一个日志等级，它就会禁止该级别和更低级别的所有日志消息。示例：
```python
import logging
logging.basicConfig(level = logging.INFO,format \
  = ' %(asctime)s - %(levelname)s - %(message)s')
print(' Enable log level >= INFO')
logging.debug('Debug')
logging.info('Info')
logging.critical('Critical error !')
print(' Start disable CRITICAL')
# 禁止 CRITICAL 级别和更低级别的所有日志消息
logging.disable(logging.CRITICAL)
logging.critical('Critical error !')
logging.error('Error!')
```
Run it,show:
```log
 Enable log level >= INFO
 2021-02-28 12:47:52,543 - INFO - Info
 2021-02-28 12:47:52,543 - CRITICAL - Critical error !
 Start disable CRITICAL
```

### 将日志记录到文件
logging.basicConfig() 函数接受 filename 关键字参数，像这样：
```python
logging.basicConfig(filename='myProgramLog.txt'\
  ,level = logging.DEBUG,format \
  = ' %(asctime)s - %(levelname)s - %(message)s')
```
此时，日志信息将写入到文件，屏幕上将不再显示日志信息。

- [上一级](README.md)
- 上一篇 -> [Python核心编程-网络编程](corePythonNet.md)
- 下一篇 -> [普通用户安装 python package](noRootUserInstallPackage.md)
