# mysql提示“Lock wait timeout exceeded; try restarting transaction”的解决办法

## Force unlock for locked tables

  1.enter mysql
```shell
  mysql -u your_user -p
```


  2.list of locked tables
```sql
  show open tables where in_use>0;
```
  3.list of the current processes,one of threm is locking your table(s)
  ```sql
  show processlist;
  --show custom processlist mysql >=5.1.7 version or use shell grep;
  select * from information_schema.proprocesslist;
  ```


  4.kill one of threse processes
  ```
  kill '..process_id..'
  ```

  5.show affected table(s)

  ```sql
  show engine innodb status\G
  ```

## innodb_lock_wait_timeout

```sql
show variables like 'innodb_lock_wait_timeout';
```

You can set it to higher value in /etc/my.cnf permanently with this line

```
[mysqld]
innodb_lock_wait_timeout=120
```
and restart mysql,if you cannot restart mysql at this time,run
```
SET GLOBAL innodb_lock_wait_timeout = 120;
```
原文链接
[how to customize 'show processlist' in mysql?](https://stackoverflow.com/questions/929612/how-to-customize-show-processlist-in-mysql)

[  How to debug Lock wait timeout exceeded on MySQL? ](https://stackoverflow.com/questions/6000336/how-to-debug-lock-wait-timeout-exceeded-on-mysql)

[Getting “Lock wait timeout exceeded; try restarting transaction” even though I'm not using a transaction](https://stackoverflow.com/questions/5836623/getting-lock-wait-timeout-exceeded-try-restarting-transaction-even-though-im)





[上一级](base.md)
[下一篇](delete_data.md)
