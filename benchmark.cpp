#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <iostream>
#include <cstring>

#include "ThreadPool.h"

#define ERR_ARG    1
#define ERR_NOMEM  2
#define ERR_SOCKET 3

static const char* errArg = "usage ./benchmark [ip] [port] [threadNum] [socketNum] [requestNum]";

static int iThreadNum  = -1;
static int iSocketNum  = -1;
static int iRequestNum = -1;

static in_addr_t iIp = 0;
static int iPort = 0;
static sockaddr_in* pServer = nullptr;

typedef struct Result {
  int iSuccess;
  long long lTime;
} result_t ;

void exitErr(const char* info, int ret) {
  std::cerr << info << std::endl;
  exit(ret);
}

result_t client(int socketNum, int requestNum) {
  result_t ret;
  std::vector<int> fds(socketNum, 0);

  for(int i = 0; i < socketNum; ++i) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) { exitErr("socket create error", ERR_SOCKET); }
    fds[i] = fd;
  }

  const char buffer[] = "HELLOWORLD";
  char recvBuffer[4096];

  auto startT = std::chrono::system_clock::now();
  for(int i = 0; i < socketNum; ++i) {
    if(connect(fds[i], (struct sockaddr*)pServer, sizeof(*pServer))) {
      std::cerr << "connet error fd:" << fds[i] << std::endl;
    }
    send(fds[i], buffer, sizeof(buffer), 0);
    recv(fds[i], (void*)recvBuffer, 4096, 0);
    if(!strcmp(buffer, recvBuffer)) { ret.iSuccess++; }
    memset(recvBuffer, 0, sizeof(recvBuffer));
    close(fds[i]);
  }
  auto endT = std::chrono::system_clock::now();

  ret.lTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endT - startT).count();
  return ret;
}

int main(int argc, char *argv[]) {
  char sIp[16] = {0};
  if(argc < 6) { exitErr(errArg, ERR_ARG); }
  // 解析链接ip
  sscanf(argv[1], "%s", sIp);
  iIp = inet_addr(sIp);
  if(iIp == 0) { exitErr(errArg, ERR_ARG); }
  // 解析链接端口
  sscanf(argv[2], "%d", &iPort);
  if(iPort == 0) { exitErr(errArg, ERR_ARG); }
  // 解析线程数
  sscanf(argv[3], "%d", &iThreadNum);
  if(iThreadNum == -1) { exitErr(errArg, ERR_ARG); }
  // 解析每个线程同时处理链接数
  sscanf(argv[4], "%d", &iSocketNum);
  if(iSocketNum == -1) { exitErr(errArg, ERR_ARG); }
  // 解析每个线程处理连接总数
  sscanf(argv[5], "%d", &iRequestNum);
  if(iRequestNum == -1) { exitErr(errArg, ERR_ARG); }
  std::cout << 
      "connect to:" << argv[1] << ":" << argv[2] << 
      "\ncreate thread:         " << argv[3] << 
      "\nthread open socket:    " << argv[4] << 
      "\nsocket handle request: " << argv[5] << std::endl;
  // 创建链接地址
  pServer = (struct sockaddr_in*)calloc(1, sizeof(*pServer));
  if(pServer == nullptr) { exitErr(errArg, ERR_ARG); }
  pServer->sin_family      = AF_INET;
  pServer->sin_port        = htons(iPort);
  pServer->sin_addr.s_addr = iIp;

  std::vector<std::future<result_t>> rets;
  ThreadPool tp(iThreadNum);
  for(int i = 0; i < iThreadNum; ++i) {
    rets.push_back(tp.addTask(client, iSocketNum, iRequestNum));
  }
  for(int i = 0; i <iThreadNum; ++i) {
    auto result = rets[i].get();
    std::cout << "thread " << i << ":" << result.iSuccess << " " << result.lTime << std::endl;
  }
  return 0;
}
