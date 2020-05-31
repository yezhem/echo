# 编译
```
$ make
```
# 客户端
```
$ ./client [ip] [port]
ip   - 链接 ip 地址
port - 链接端口号
```
# 服务端
```
$ ./server_select [port]
port - 链接端口号
```
# 基准测试
```
$ ./benchmark [ip] [port] [threadNum] [socketNum] [requestNum]
ip   - 链接 ip 地址
port - 链接端口号
threadNum  - 开启线程数
socketNum  - 每个线程链接数
requestNum - 每个链接发送多少次请求
```
# 测试结果
|         模式         | 平均时延（s） |
| :------------------: | :-----------: |
|        单线程        |       -       |
|        多线程        |    32.678     |
| 线程池（1024线程数） |     0.029     |
|      忙等待轮询      |    68.172     |
|        select        |    41.699     |
# 说明
[echo服务端](http://hh-yzm.com/index.php/archives/47/)
