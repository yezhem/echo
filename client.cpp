#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <iostream>
#include <cstring>

#define RUN_OK      0
#define ERR_ARG     1
#define ERR_IP      2
#define ERR_PORT    3
#define ERR_NOMEM   4
#define ERR_SOCKET  5
#define ERR_CONNECT 6

#define MAX_BUFFER 4096

static const char* program = __FILE__;
static const char* errArg = "error argument, to using ./client [ip] [port]\n";

int run(int fd) {
  char buffer[MAX_BUFFER];
  char sIn[MAX_BUFFER];
  while(true) {
    std::cout << "> ";
    std::cin.getline(sIn, sizeof(sIn));
    // 发送消息
    ssize_t sSend = send(fd, sIn, strlen(sIn), 0);
    // 接收消息
    ssize_t sRecv = recv(fd, (void*)buffer, MAX_BUFFER, 0);
    buffer[sRecv] = '\0';
    std::cout << "- " << buffer << std::endl;
  }
  return RUN_OK;
}

int main(int argc, char *argv[]) {
  struct sockaddr_in* pServer;
  struct sockaddr* pAddr;
  int iSockfd = 0;
  int iPort = 0;
  char sIp[16] = {0};
  in_addr_t iIp = 0;
  // 解析参数
  if(argc < 3) {
    const char *pos = strchr(program, '.');
    char cmd[32];
    strcpy(cmd, program);
    cmd[pos - program] = '\0';
    std::cerr << errArg;
    exit(ERR_ARG);
  }
  // 解析链接ip
  sscanf(argv[1], "%s", sIp);
  iIp = inet_addr(sIp);
  if(iIp == 0) {
    std::cerr << errArg;
    exit(ERR_ARG);
  }
  // 解析链接端口
  sscanf(argv[2], "%d", &iPort);
  if(iPort == 0) {
    std::cerr << errArg;
    exit(ERR_ARG);
  }
  // 创建套接字
  iSockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(iSockfd < 0) {
    exit(ERR_SOCKET);
  }
  // 创建链接地址
  pServer = (struct sockaddr_in*)calloc(1, sizeof(*pServer));
  if(pServer == nullptr) {
    exit(ERR_NOMEM);
  }
  pServer->sin_family      = AF_INET;
  pServer->sin_port        = htons(iPort);
  pServer->sin_addr.s_addr = iIp;
  pAddr = (struct sockaddr*)pServer;
  // 链接
  if(connect(iSockfd, pAddr, sizeof(*pServer))) {
    close(iSockfd);
    exit(ERR_CONNECT);
  }
  // 运行
  return run(iSockfd);
}
