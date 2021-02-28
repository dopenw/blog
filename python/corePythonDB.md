# Python核心编程-数据库编程

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Python核心编程-数据库编程](#python核心编程-数据库编程)
  - [一个简单的 sqlite3 demo](#一个简单的-sqlite3-demo)

<!-- /code_chunk_output -->

## 一个简单的 sqlite3 demo
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
`('john', 100)
- [上一级](README.md)
- 上一篇 -> [Python 核心编程:正则表达式](CorePython.md)
- 下一篇 -> [Python核心编程-网络编程](corePythonNet.md)
