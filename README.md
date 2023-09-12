# funny-project
some funny project
## 1. Project to get weather and soul tip from jinshan, shanbay or youdao
## 2. 随机生成毒鸡汤
下载毒鸡汤数据库https://github.com/egotong/nows, 下载工具将原mysql转化为
```b
wget https://github.com/dumblob/mysql2sqlite/archive/refs/heads/master.zip
unzip master.zip
cd mysql2sqlite-master/
./mysql2sqlite soul.sql | sqlite3 sqlite.db
```
