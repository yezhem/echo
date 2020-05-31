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
