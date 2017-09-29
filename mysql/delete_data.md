# 删除数据量较大的表的部分数据

1.使用小量多次delete的方法
```shell

while [ 1 -eq 1 ]
do
mysql -uUser -pPassword -Ddatabase -hHost << EOF
delete from Table where (codition==True) limit 1000(or other);
exit
EOF
done

```
该方法删除数据慢，删除后物理空间也并没有释放，附：使用delete删除的时候，mysql并没有把数据文件删除，而是将数据文件的标识位删除，没有整理文件，因此不会彻底释放空间。被删除的数据将会被保存在一个链接清单中，当有新数据写入的时候，mysql会利用这些已删除的空间再写入。即，删除操作会带来一些数据碎片，正是这些碎片在占用硬盘空间。

2.将需要的数据抽到临时表中，再删除表。该方法效率较高
```sql
insert into tmp_table select * from Table where (codition=True);
-- 遇到的问题：命令执行完后，select * from tmp_table结果无数据，查看物理空间发现有几个G的数据，可能是数据量太大造成insert失败，考虑select数据量分小一点。
```












































[上一级](base.md)
[上一篇](Lock_wait_timeout_exceeded.md)
