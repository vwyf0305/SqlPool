# 简单实现一个数据库连接池
基于[该视频](https://www.bilibili.com/video/BV1Fr4y1s7w4/?spm_id_from=333.337.search-card.all.click&vd_source=7b3a60d5f798acf57770a5066a196d3f)在Linux上简单实现了一个数据库连接池，采用xmake进行构建。

## 运行

服务器环境：
* Fedora36
* MySQL8.0.30

使用的外部库
* MySQL
* jsoncpp
* fmt

编译:
```
cd ProjectFiles\
xmake build
```

生成CMakeLists.txt:
```
xmake -k cmake -y
```

运行:
```
xmake run
```