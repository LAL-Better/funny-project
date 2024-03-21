# funny-project
some funny project
## 1. Project to get weather and soul tip from jinshan, shanbay or youdao
需要注册高德开发者的key
## 2. 随机生成毒鸡汤
下载毒鸡汤数据库https://github.com/egotong/nows, 下载工具将原mysql转化为
```b
wget https://github.com/dumblob/mysql2sqlite/archive/refs/heads/master.zip
unzip master.zip
cd mysql2sqlite-master/
./mysql2sqlite soul.sql | sqlite3 sqlite.db
```
## 4. 智能车项目
首先mqtt实现微信小程序与树莓派的通信，ROS系统实现树莓派对小车的控制
1. mqtt 服务器搭建
https://www.cnblogs.com/cg-ww/p/16165101.html#%E5%9B%9Bmqtt-demo
2. 示例程序s
https://blog.csdn.net/XC_SunnyBoy/article/details/115790445
