#include "common.h"

static const char* program  = __FILE__;

// 服务端处理函数
int doEcho(int fd) {
  char buffer[MAX_BUFFER];
  while(true) {
    ssize_t sRecv = recv(fd, (void*)buffer, MAX_BUFFER, 0);
    if(sRecv == 0) {
      break;
    }
    if(sRecv == -1) {
      exit(ERR_RECV);
    }
    send(fd, buffer, sRecv, 0);
  }
  close(fd);
  return RUN_OK;
}

// 主函数
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
    std::cerr << "usage: ./" << cmd << " [port]" << std::endl;
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
  if(listen(iSockfd, BACKLOG) < 0) {
    exit(ERR_LISTEN);
  }

  run(iSockfd);

  close(iSockfd);
  return 0;
}
