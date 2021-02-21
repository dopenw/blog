# sqlite 学习


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [sqlite 学习](#sqlite-学习)
	* [sqlite 官网](#sqlite-官网)
	* [tutorialspoint sqlite](#tutorialspoint-sqlite)
	* [Cross-platform open source sqlite gui](#cross-platform-open-source-sqlite-gui)
	* [sqlite 简介](#sqlite-简介)
	* [sqlite 特征](#sqlite-特征)
	* [sqlite help](#sqlite-help)
	* [sqlite Format output](#sqlite-format-output)
	* [sqlite 语法](#sqlite-语法)
	* [sqlite c/c++](#sqlite-cc)
	* [参考链接](#参考链接)

<!-- /code_chunk_output -->

## sqlite 官网
[sqlite home](https://www.sqlite.org/index.html)

## tutorialspoint sqlite
[tutorialspoint/sqlite](https://www.tutorialspoint.com/sqlite/)

## Cross-platform open source sqlite gui
[DB Browser for SQLite](https://sqlitebrowser.org/)

## sqlite 简介
SQLite 是遵守 [ACID](https://zh.wikipedia.org/wiki/ACID) 的 [关系数据库](https://zh.wikipedia.org/wiki/%E5%85%B3%E7%B3%BB%E6%95%B0%E6%8D%AE%E5%BA%93) 管理系统，它包含在一个相对小的C程序库中。与许多其它数据库管理系统不同，SQLite不是一个客户端/服务器结构的数据库引擎，而是被集成在用户程序中。

## sqlite 特征
程序库实现了多数的SQL-92标准，包括事务，以及实现原子性、一致性、隔离性和持久性（即ACID），但它缺失了某些特性。例如，它仅部分支持触发器。尽管它支持大多数的复杂查询，但它的ALTER TABLE功能有所限制，不能修改或删除列，只能通过重新创建表的方式迂回进行。

SQLite不进行类型检查。你可以把字符串插入到整数列中。某些用户发现这是使数据库更加有用的创新，特别是与无类型的脚本语言一起使用的时候；然而其他用户认为这是主要的缺点。

多个进程或线程可以同时访问同一个数据而没有问题。可以同时平行读取同一个数据库。但同一时间只能有一个进程或线程进行数据写入；否则会写入失败并得到一个错误消息（或者会自动重试一段时间；自动重试的逻辑以及重试时间的长短是可以设置的）。

程序设计者还提供了一个叫做sqlite3的独立程序用来查询和管理SQLite数据库文件。SQLite的用户可以把这个程序当作如何写SQLite应用程序的示例。
There are few unsupported features of SQL92 in SQLite which are listed in the following table:

| No |                                                                              Feature                                                                             |
|--------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|    1   | RIGHT OUTER JOIN , Only LEFT OUTER JOIN is implemented.                                                                                                           |
|    2   | FULL OUTER JOIN , Only LEFT OUTER JOIN is implemented.                                                                                                            |
|    3   | ALTER TABLE , The RENAME TABLE and ADD COLUMN variants of the ALTER TABLE command are supported. The DROP COLUMN, ALTER COLUMN, ADD CONSTRAINT are not supported. |
|    4   | Trigger support , FOR EACH ROW triggers are supported but not FOR EACH STATEMENT triggers.                                                                        |
|    5   | VIEWs , VIEWs in SQLite are read-only. You may not execute a DELETE, INSERT, or UPDATE statement on a view.                                                       |
|    6   | GRANT and REVOKE , The only access permissions that can be applied are the normal file access permissions of the underlying operating system.                     |

## sqlite help
```sh
[breap@breap ~]$ sqlite3
SQLite version 3.22.0 2018-01-22 18:45:57
Enter ".help" for usage hints.
Connected to a transient in-memory database.
Use ".open FILENAME" to reopen on a persistent database.
sqlite> .help
.archive ...           Manage SQL archives: ".archive --help" for details
.auth ON|OFF           Show authorizer callbacks
.backup ?DB? FILE      Backup DB (default "main") to FILE
.bail on|off           Stop after hitting an error.  Default OFF
.binary on|off         Turn binary output on or off.  Default OFF
.cd DIRECTORY          Change the working directory to DIRECTORY
.changes on|off        Show number of rows changed by SQL
.check GLOB            Fail if output since .testcase does not match
.clone NEWDB           Clone data into NEWDB from the existing database
.databases             List names and files of attached databases
.dbinfo ?DB?           Show status information about the database
.dump ?TABLE? ...      Dump the database in an SQL text format
                         If TABLE specified, only dump tables matching
                         LIKE pattern TABLE.
.echo on|off           Turn command echo on or off
.eqp on|off|full       Enable or disable automatic EXPLAIN QUERY PLAN
.excel                 Display the output of next command in a spreadsheet
.exit                  Exit this program
.expert                EXPERIMENTAL. Suggest indexes for specified queries
.fullschema ?--indent? Show schema and the content of sqlite_stat tables
.headers on|off        Turn display of headers on or off
.help                  Show this message
.import FILE TABLE     Import data from FILE into TABLE
.imposter INDEX TABLE  Create imposter table TABLE on index INDEX
.indexes ?TABLE?       Show names of all indexes
                         If TABLE specified, only show indexes for tables
                         matching LIKE pattern TABLE.
.limit ?LIMIT? ?VAL?   Display or change the value of an SQLITE_LIMIT
.lint OPTIONS          Report potential schema issues. Options:
                         fkey-indexes     Find missing foreign key indexes
.load FILE ?ENTRY?     Load an extension library
.log FILE|off          Turn logging on or off.  FILE can be stderr/stdout
.mode MODE ?TABLE?     Set output mode where MODE is one of:
                         ascii    Columns/rows delimited by 0x1F and 0x1E
                         csv      Comma-separated values
                         column   Left-aligned columns.  (See .width)
                         html     HTML <table> code
                         insert   SQL insert statements for TABLE
                         line     One value per line
                         list     Values delimited by "|"
                         quote    Escape answers as for SQL
                         tabs     Tab-separated values
                         tcl      TCL list elements
.nullvalue STRING      Use STRING in place of NULL values
.once (-e|-x|FILE)     Output for the next SQL command only to FILE
                         or invoke system text editor (-e) or spreadsheet (-x)
                         on the output.
.open ?OPTIONS? ?FILE? Close existing database and reopen FILE
                         The --new option starts with an empty file
.output ?FILE?         Send output to FILE or stdout
.print STRING...       Print literal STRING
.prompt MAIN CONTINUE  Replace the standard prompts
.quit                  Exit this program
.read FILENAME         Execute SQL in FILENAME
.restore ?DB? FILE     Restore content of DB (default "main") from FILE
.save FILE             Write in-memory database into FILE
.scanstats on|off      Turn sqlite3_stmt_scanstatus() metrics on or off
.schema ?PATTERN?      Show the CREATE statements matching PATTERN
                          Add --indent for pretty-printing
.selftest ?--init?     Run tests defined in the SELFTEST table
.separator COL ?ROW?   Change the column separator and optionally the row
                         separator for both the output mode and .import
.sha3sum ?OPTIONS...?  Compute a SHA3 hash of database content
.shell CMD ARGS...     Run CMD ARGS... in a system shell
.show                  Show the current values for various settings
.stats ?on|off?        Show stats or turn stats on or off
.system CMD ARGS...    Run CMD ARGS... in a system shell
.tables ?TABLE?        List names of tables
                         If TABLE specified, only list tables matching
                         LIKE pattern TABLE.
.testcase NAME         Begin redirecting output to 'testcase-out.txt'
.timeout MS            Try opening locked tables for MS milliseconds
.timer on|off          Turn SQL timer on or off
.trace FILE|off        Output each SQL statement as it is run
.vfsinfo ?AUX?         Information about the top-level VFS
.vfslist               List all available VFSes
.vfsname ?AUX?         Print the name of the VFS stack
.width NUM1 NUM2 ...   Set column widths for "column" mode
                         Negative values right-justify
sqlite>
```

## sqlite Format output
```sh
sqlite>.header on
sqlite>.mode column
sqlite>.timer on
sqlite>
```

## sqlite 语法
详见 [tutorialspoint/sqlite/Syntax](https://www.tutorialspoint.com/sqlite/sqlite_syntax.htm)章节；

## sqlite c/c++
[tutorialspoint/SQLite - C/C++](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)

select 示例：
test table :
```sql
sqlite> .schema test
CREATE TABLE test(name TEXT,age int);
```

[Difference Between “Text” and “String” datatype in SQLite](https://stackoverflow.com/questions/11938401/difference-between-text-and-string-datatype-in-sqlite)

```c++
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);

   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "SELECT * from test";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
```

Run it:
```sh
[breap@breap ~]$ ./sqliteSelect
Opened database successfully
Callback function called: name = Tom
age = 20

Callback function called: name = John
age = 23

Operation done successfully
```

## 参考链接
* [SQLite wikipedia](https://zh.wikipedia.org/wiki/SQLite)
* [tutorialspoint/sqlite](https://www.tutorialspoint.com/sqlite/sqlite_overview.htm)
* [configure: error: SQLite3 library not found](https://dba.stackexchange.com/questions/52336/configure-error-sqlite3-library-not-found)

[上一级](README.md)
[上一篇 -> 排序](sort.md)
[下一篇 -> 数据结构（树）](tree.md)
