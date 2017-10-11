
* [安装配置hadoop httpfs](#安装配置hadoop-httpfs)
	* [1. Configure HttpFS](#1-configure-httpfs)
	* [2. Configure Hadoop](#2-configure-hadoop)
	* [3. stop hadoop](#3-stop-hadoop)
	* [4. start httpfs](#4-start-httpfs)
	* [5. start dfs](#5-start-dfs)
	* [6. test httpfs is working](#6-test-httpfs-is-working)


# 安装配置hadoop httpfs

* 基于2.8.1版本

## 1. Configure HttpFS

* path:   $HADOOP_HOME/etc/hadoop

By default, HttpFS assumes that Hadoop configuration files (core-site.xml & hdfs-site.xml) are in the HttpFS configuration directory.

If this is not the case, add to the httpfs-site.xml file the httpfs.hadoop.config.dir property set to the location of the Hadoop configuration directory.

## 2. Configure Hadoop

Edit Hadoop core-site.xml and defined the Unix user that will run the HttpFS server as a proxyuser. For example:

```
<property>
<name>hadoop.proxyuser.httpfs.hosts</name>
<value>*</value>
</property>
<property>
<name>hadoop.proxyuser.httpfs.groups</name>
<value>*</value>
</property>
```

## 3. stop hadoop

## 4. start httpfs

* path: $HADOOP_HOME/sbin

```shell
./httpfs.sh start
```

## 5. start dfs

* path: $HADOOP_HOME/sbin

```shell
./start_dfs.sh
```

## 6. test httpfs is working

```shell
curl -sS 'http://<HTTPFSHOSTNAME>:14000/webhdfs/v1?op=gethomedirectory&user.name=hdfs'
{"Path":"\/user\/hdfs"}
```



[上一级](base.md)
[下一篇](docker.md)
