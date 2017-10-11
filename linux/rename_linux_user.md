# 更改linux用户名

## method 1:
1. "ctrl + alt + f3",login root
```shell
systemctl stop gdm.service
```

2. then
```shell
ps aux | grep -i "systemd --user"
```

3. kill user_name  process
```shell
kill -s 9 $process_id
```

4. rename
```shell
usermod -l $new_name $old_name
```

## method 2:

1. "ctrl + alt + f3",login root
```shell
systemctl stop gdm.service
```

2. then
```shell
ps aux | grep -i "systemd --user"
```

3. kill user_name  process
```shell
kill -s 9 $process_id
```

4. move user home directory to new user directory
```shell
usermod --move-home --home /home/$new_user $old_user
```

5. create new user
```shell
useradd --home /home/$new_user $new_user
```

6. change who owns the folder:
```shell
chown -R $new_user:$new_user /home/$new_user
```

7. delete the old user if needed

```shell
userdel $old_user
```


[source link](https://ask.fedoraproject.org/en/question/55339/change-of-user-name-at-terminal/)






[上一级](base.md)
[上一篇](process.md)
