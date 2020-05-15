#include "common.h"

#include <thread>

const char* program  = __FILE__;

// multi thread server - 多线程模式
int run(int iSockfd) {
  // 循环创建链接
  while(true) {
    int fd = accept(iSockfd, nullptr, nullptr);
    if(fd < 0) {
      exit(ERR_ACCEPT);
    }
    std::thread tEcho(doEcho, fd);
    tEcho.detach();
  }
  return RUN_OK;
}
