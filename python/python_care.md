# python记录

## python数学操作符：
1. ** 指数
2. / 除法 ，eg：22/8 -> 2.75
3. // 整除/商数取整 22/8 -> 2

## 注释
使用 ‘#’符号
eg：
```python
# this is test
```

## 字典

1. get（）方法：在访问一个键之前，检查该键是否存在于字典中，这很麻烦，好在有个get（）方法，他有两个参数：要取得其值的键，以及如果该键不存在时，返回的备用值。
eg：
```python
test={'oo':1}
test.get('kk',0)
```

2. 你常常需要为字典中的某个键设置一个默认值，当该键没有任何值时使用它,代码看起来相这样：
```python
spam={'name':'pamama'}
if 'color' not in spam:
  spam['color']='black'
```
setdefault()方法提供了一种形式，在一行中完成这样的事情。
```python
spam.setdefault('color',black)
```

3. 漂亮打印
程序中导入pprint模块，就可以使用pprint(),pformat()函数了，他们将漂亮打印一个字典的字。

### 字符串操作

1. 字符串方法join（）和split（）

如果有一个字符串列表，需要将他们连接起来，成为一个单独的字符串，join方法就很有用。

eg：
```python
','.join(['tes','fd','fds'])

#output
'tes,fd,fds'
```

2. split() method:
```python
"Test split method".split()

#output
['Test', 'split', 'method']
```

3. 用pyperclip模块拷贝粘贴字符串
pyperclip模块有copy()和paste()函数，可以向计算机的剪贴板发送文本，或从它接收文本。需要自行安装pyperclip模块。
```python
import pyperclip
pyperclip.copy('Hello world')
pyperclip.paste()
#output
'Hello world'
```
---
- [上一级](README.md)
- 上一篇 -> [普通用户安装 python package](noRootUserInstallPackage.md)
- 下一篇 -> [翻译小工具](translateTool.md)
