#include "common.h"

#include "ThreadPool.h"

static const int iThread = 16;

// thread pool server - 线程池模式
int run(int iSockfd) {
  ThreadPool tp(iThread);
  while(true) {
    int fd = accept(iSockfd, nullptr, nullptr);
    if(fd < 0) {
      exit(ERR_ACCEPT);
    }
    tp.addTask(doEcho, fd);
  }
  return RUN_OK;
}
