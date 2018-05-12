# 解决 Wine 下字体的乱码、模糊等问题

使用 wine 安装一些windows程序之后（例如 enterprise architect ），启动程序后发现字体很细、模糊，修改注册表也没有得到改善。实际上是缺少相应的字体，只要把 windows 下的字体拷贝到 linux 下面即可。

解决方法：
```sh
# copy windows system C://windows/Fonts (TTC and TTF font format) to $HOME/.font
# then run
mkfontscale
mkfontsdir
```

最后重启系统即可


Link:
* [解决linux下flash,qq各种乱码 字体模糊 的不太合法的方案](https://blog.csdn.net/xcwenn/article/details/20701411)

[上一级](base.md)
[上一篇](su_diff_su-.md)
