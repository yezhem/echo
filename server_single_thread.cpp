#include "common.h"

const char* program  = __FILE__;

// single thread server - 单线程模式
int run(int iSockfd) {
  // 循环创建链接
  while(true) {
    int fd = accept(iSockfd, nullptr, nullptr);
    if(fd < 0) {
      exit(ERR_ACCEPT);
    }
    doEcho(fd);
  }
  return RUN_OK;
}
