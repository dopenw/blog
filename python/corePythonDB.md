# Python核心编程-数据库编程

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Python核心编程-数据库编程](#python核心编程-数据库编程)
  - [sqlite](#sqlite)
    - [一个简单的demo](#一个简单的demo)
    - [添加 blob 数据demo](#添加-blob-数据demo)

<!-- /code_chunk_output -->

## sqlite

### 一个简单的demo

```python
import sqlite3
cxn = sqlite3.connect('sqlite_test/test')
cur = cxn.cursor()
cur.execute('CREATE TABLE users(login VARCHAR(8)\
    ,userid INTEGER)')

cur.execute('INSERT INTO users VALUES("john",100)')
cur.execute('INSERT INTO users VALUES("jane",110)')
cur.execute('SELECT * FROM users')
for eachUser in cur.fetchall():
    print(eachUser) 

cur.execute('DROP TABLE users')

cur.close()
cxn.commit()
cxn.close()
```

Run it,show:
`('john', 100)`

### 添加 blob 数据demo

```py
import sqlite3
import array

# double 数组 -> bytes
test_data = bytes(array.array('d',[0.0,]*1025))

cxn = sqlite3.connect('test.db')
cur = cxn.cursor()
cur.execute('''CREATE TABLE IF NOT EXISTS Test (
 ID INTEGER,
 info INTEGER,
 blob_data BLOB
)''')

cur.execute('''INSERT INTO Test(ID,info,blob_data) VALUES({},"{}",?)'''\
  .format(1,'test'),(test_data,))

cur.execute('SELECT * FROM Test')
for row in cur.fetchall():
    print(row[0])
    print(row[1])
    print(type(row[2]))


cur.close()
cxn.commit()
cxn.close()
```

Run it,show:

```cmd
1
test
<class 'bytes'>
```

详见 [Python SQLite BLOB to Insert and Retrieve file and images](https://pynative.com/python-sqlite-blob-insert-and-retrieve-digital-data/)

---
- [上一级](README.md)
- 上一篇 -> [Python 核心编程:正则表达式](CorePython.md)
- 下一篇 -> [Python核心编程-网络编程](corePythonNet.md)
