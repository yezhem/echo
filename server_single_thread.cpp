#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

#define RUN_OK     0
#define ERR_ARG    1
#define ERR_BIND   2
#define ERR_FD     3
#define ERR_LISTEN 4
#define ERR_NOMEM  5
#define ERR_ACCEPT 6
#define ERR_RECV   7

#define MAX_BUFFER 4096

static const char* program = __FILE__;

// single thread server - 单线程模式
int run(int iSockfd) {
  char buffer[4096];
  // 循环创建链接
  while(true) {
    struct sockaddr_in sClient;
    socklen_t sLen = sizeof(sClient);
    int fd = accept(iSockfd, (struct sockaddr*)&sClient, &sLen);
    if(fd < 0) {
      exit(ERR_ACCEPT);
    }
    std::cout << "accept the port:" << sClient.sin_port << std::endl;
    while(true) {
      ssize_t sRecv = recv(fd, (void*)buffer, MAX_BUFFER, 0);
      if(sRecv == 0) {
        std::cout << "close the port:" << sClient.sin_port << std::endl;
        break;
      }
      if(sRecv == -1) {
        exit(ERR_RECV);
      }
      ssize_t sSend = send(fd, buffer, sRecv,0);
    }
    close(fd);
  }
  return RUN_OK;
}

int main(int argc, char *argv[]) {
  int iPort;
  int iSockfd;
  struct sockaddr* plistenAddr;
  struct sockaddr_in* pInAddr;

  // 读取参数
  if(argc < 2) {
    char cmd[32];
    const char *pos = strchr(program, '.');
    strcpy(cmd, program);
    cmd[pos - program] = '\0';
    std::cerr << "error argument, to using:./" << cmd << " [port]" << std::endl;
    exit(ERR_ARG);
  }
  // 读取端口
  sscanf(argv[1], "%d", &iPort);
  if(iPort == 0) {
    exit(ERR_ARG);
  }
  // 创建 socket 文件句柄
  iSockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(iSockfd < 0) {
    exit(ERR_FD);
  }
  // 监听地址
  pInAddr = (struct sockaddr_in*)calloc(1, sizeof(*pInAddr));
  if(pInAddr == nullptr) {
    exit(ERR_NOMEM);
  }
  pInAddr->sin_family      = AF_INET;
  pInAddr->sin_addr.s_addr = 0;
  pInAddr->sin_port        = htons(iPort);
  plistenAddr = (struct sockaddr*)pInAddr;
  // 将文件句柄与监听地址绑定
  if(bind(iSockfd, plistenAddr, sizeof(*pInAddr)) < 0) {
    exit(ERR_BIND);
  }
  // 开始监听
  if(listen(iSockfd, 16) < 0) {
    exit(ERR_LISTEN);
  }

  run(iSockfd);

  close(iSockfd);
  return 0;
}
