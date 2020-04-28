# 13. 数据库

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [13. 数据库](#13-数据库)
  - [连接和查询](#连接和查询)
  - [Link](#link)

<!-- /code_chunk_output -->


[QtSql](https://doc.qt.io/qt-5/sql-programming.html) 提供了与平台及数据库种类无关的访问 SQL 数据库地接口。这个接口由利用 Qt 的模型/视图结构将数据库与用户界面集成的一套类来支持。

[QSqlDatabase](https://doc.qt.io/qt-5/qsqldatabase.html) 对象表征了数据库的关联。Qt 使用驱动程序与各种数据库的应用程序编程接口进行通信。Qt 的桌面版(Desktop Edition) 包括如下一些驱动程序：

| Driver Type |                 Description                 |
|:-----------:|:-------------------------------------------:|
|     QDB2    |                   IBM DB2                   |
|    QIBASE   |           Borland InterBase Driver          |
|    QMYSQL   |                 MySQL Driver                |
|     QOCI    |         Oracle Call Interface Driver        |
|    QODBC    | ODBC Driver (includes Microsoft SQL Server) |
|    QPSQL    |              PostgreSQL Driver              |
|   QSQLITE   |          SQLite version 3 or above          |
|   QSQLITE2  |               SQLite version 2              |
|     QTDS    |            Sybase Adaptive Server           |

对于那些习惯了 SQL 语法的用户， [QSqlQuery](https://doc.qt.io/qt-5/qsqlquery.html) 提供了一种直接执行任意的 SQL 语句并处理其结果的方式。对于那些喜欢更高级、更友好的数据库界面以避免 SQL 语法的用户， [QSqlTableModel](https://doc.qt.io/qt-5/qsqltablemodel.html) 和 [QSqlRelationalTableModel](https://doc.qt.io/qt-5/qsqlrelationaltablemodel.html) 提供了合适的抽象。这些类以与其他 Qt 模型类相同的方式来表示一个 SQL 表。它们可以被单独用来遍历和编辑程序代码中的数据，也可以添加最终用户能够查看并修改数据的视图。

Qt使对常见数据库特性的编程变得简单易懂，例如对主从数据库和下钻型(drill-down)数据库的编程；另外，Qt 也让利用窗体或图形用户界面中的表查看数据库表的过程变得简单易行。

## 连接和查询

为了执行 SQL 查询，首先必须建立与数据库的连接。eg：
```c++
bool createConnection()
{
  // 创建 QSqlDatabase 对象，这里指定使用 MySQL
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("mozart.konkordia.edu");
  db.setDatabaseName("musicdb");
  db.setUserName("gbatstone");
  db.setPassword("T17aV44");
  if (!db.open()) {
    QMessageBox::critical(0, QObject::tr("Database Error"),
    db.lastError().text());
    return false;
  }
  return true;
}
```

一旦连接建立，就可以使用 QSqlQuery 执行底层数据库支持的任何 SQL 语句了。eg：
```c++
QSqlQuery query;
query.exec("SELECT title, year FROM cd WHERE year >= 1998");

// 等价于

QSqlQuery query("SELECT title, year FROM cd WHERE year >= 1998");

//遍历查询的结果集

while (query.next()) {
  // value() 函数把字段作为 QVariant 返回
  QString title = query.value(0).toString();
  int year = query.value(1).toInt();
  std::cerr << qPrintable(title) << ": " << year << std::endl;
}
```
只要调用 next() 一次，就可以把这个 QSqlQuery 定位到结果集中的第一条记录。随后的 next() 调用，每次都会把记录指针前移一条记录，直到到达结尾时 next() 才返回 false。如果结果集为空（或查询失败），那么 next() 的第一次调用将返回 false。

QSqlQuery 提供了一些可以遍历结果集的函数：first() 、 last() 、 previous() 和 seek() 。这些函数都很方便，不过对于某些数据库，它们可能会比 next() 更慢或者更加耗费内存。在操作一个大数据集时，为了便于优化，可以在调用 exec() 之前调用 [QSqlQuery::setForwardOnly(true)](https://doc.qt.io/qt-5/qsqlquery.html#setForwardOnly),然后只使用 next() 遍历结果集。

我们可以通过对查询调用 isActive() 来检查是否有错误发生：
```c++
if (!query.isActive())
  QMessageBox::warning(this, tr("Database Error"),
          query.lastError().text());
```

执行 insert:
```c++
QSqlQuery query("INSERT INTO cd (id, artistid, title, year) "
        "VALUES (203, 102, 'Living in America', 2002)");
```
在这之后，[numRowsAffected()](https://doc.qt.io/qt-5/qsqlquery.html#numRowsAffected) - Returns the number of rows affected by the result's SQL statement, or -1 if it cannot be determined. Note that for SELECT statements, the value is undefined; use size() instead. If the query is not active, -1 is returned.

如果需要插入多条记录，或者想避免将数值转换成为字符串（并且正确地转义它们），可以使用 [prepare()](https://doc.qt.io/qt-5/qsqlquery.html#prepare) 来指定一个包含占位符地查询，然后赋值绑定想插入地数值。Qt 对所有地数据库都支持 Oracle 风格和 ODBC 风格地占位符语法，如果它们可用，就使用本地支持；如果不可用，就模拟它的功能。



## Link
* [qt5-book-code/chap13/](https://github.com/mutse/qt5-book-code/tree/master/chap13)


[上一级](README.md)
[上一篇](12_inOut.md)
[下一篇](14_multiThread.md)
