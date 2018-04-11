# MYSQL 存储过程 VS 函数


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [MYSQL 存储过程 VS 函数](#mysql-存储过程-vs-函数)
	* [区别](#区别)
	* [存储过程](#存储过程)
		* [作用](#作用)
		* [存储过程示例](#存储过程示例)
	* [函数](#函数)
		* [函数示例](#函数示例)

<!-- /code_chunk_output -->

## 区别

1. 函数限制比较多，比如不能用临时表，只能用表变量．还有一些函数都不可用等等．而存储过程的限制相对就比较少。
2. 一般来说，存储过程实现的功能要复杂一点，而函数的实现的功能针对性比较强。存储过程，功能强大，可以执行包括修改表等一系列数据库操作；用户定义函数不能用于执行一组修改全局数据库状态的操作。
3. 对于存储过程来说可以返回参数，如记录集，而函数只能返回值或者表对象。函数只能返回一个变量；而存储过程可以返回多个。存储过程的参数可以有IN,OUT,INOUT三种类型，而函数只能有IN类~~存储过程声明时不需要返回类型，而函数声明时需要描述返回类型，且函数体中必须包含一个有效的RETURN语句。
4. 存储过程一般是作为一个独立的部分来执行（ EXECUTE 语句执行），而函数可以作为查询语句的一个部分来调用（SELECT调用），由于函数可以返回一个表对象，因此它可以在查询语句中位于FROM关键字的后面。 SQL语句中不可用存储过程，而可以使用函数。

## 存储过程

[MySQL Stored Procedure](https://www.w3resource.com/mysql/mysql-procedure.php)

[CREATE PROCEDURE and CREATE FUNCTION Syntax](https://dev.mysql.com/doc/refman/5.7/en/create-procedure.html)

### 作用

1. 存储过程因为SQL语句已经预编绎过了，因此运行的速度比较快。
2. 存储过程可以接受参数、输出参数、返回单个或多个结果集以及返回值。可以向程序返回错误原因。  
3. 存储过程主要是在服务器上运行，减少对客户机的压力。
4. 存储过程可以包含程序流、逻辑以及对数据库的查询。同时可以实体封装和隐藏了数据逻辑。
5. 存储过程可以在单个存储过程中执行一系列SQL语句。
6. 存储过程可以从自己的存储过程内引用其它存储过程，这可以简化一系列复杂语句。

### 存储过程示例

1. create a table
```sql
create table user(user varchar(30),age int);
insert into user values("tom",21);
insert into user values("jack",22);
```
2. create procedure
```sql
delimiter $$
create procedure procedure1()
  BEGIN
  select * from user;
  END
  $$
delimiter ;
```
3. call procedure
```sh
MariaDB [test]> call procedure1()；
+------+------+
| user | age  |
+------+------+
| tom  |   21 |
| jack |   22 |
+------+------+
2 rows in set (0.00 sec)

Query OK, 0 rows affected (0.00 sec)
```

4. We can divide the above CREATE PROCEDURE statement in the following ways :

```sql
1. CREATE PROCEDURE sp_name () ...

2. CREATE PROCEDURE sp_name ([IN] param_name type)...

3. CREATE PROCEDURE sp_name ([OUT] param_name type)...

4. CREATE PROCEDURE sp_name ([INOUT] param_name type)...
```

5. Parameter OUT example
```sql
delimiter $$
create procedure procedure2(OUT test int)
  begin
  select max(age) into test from user;
  end
  $$
delimiter ;
call procedure2(@M);
```
```sh
MariaDB [test]> select @M;
+------+
| @M   |
+------+
|   22 |
+------+
1 row in set (0.00 sec)
```

## 函数

[MySQL Stored Function](http://www.mysqltutorial.org/mysql-stored-function/)

[CREATE PROCEDURE and CREATE FUNCTION Syntax](https://dev.mysql.com/doc/refman/5.7/en/create-procedure.html)

### 函数示例
```sql
delimiter $$
create function function1() returns int
  begin
  declare res int;
  select max(age) into res from user;
  return res;
  end
  $$
delimiter ;
```

```sh
MariaDB [test]> select function1();
+-------------+
| function1() |
+-------------+
|          22 |
+-------------+
1 row in set (0.00 sec)
```


注：
* [MySQL 的存储过程 procedure 与 函数 function 的区别和使用方法](https://blog.csdn.net/xlxxcc/article/details/52485322)
[上一级](base.md)
[上一篇](delete_data.md)
