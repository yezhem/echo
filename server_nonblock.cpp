#include "common.h"

#include <fcntl.h>
#include <thread>

#include "SynchronizedQueue.h"

constexpr int MAX_FD = 1000;

static SynchronizedQueue<int> fdQueue(MAX_FD);

// 非阻塞应答
int doNonBlockEcho() {
  char buffer[MAX_BUFFER];
  while(true) {
    int fd = fdQueue.pop();
    ssize_t sRecv = recv(fd, buffer, sizeof(buffer), 0);
    if(sRecv == 0) {
      close(fd);
      continue;
    }else if(sRecv < 0) {
      if(errno != EAGAIN) {
        std::cerr << "error to recv" << std::endl;
        continue;
      }
    }else {
      send(fd, buffer, sRecv, 0);
    }
    fdQueue.push(fd);
  }
}

// nonblock - 非阻塞模式
int run(int iSockfd) {
  std::thread echo(doNonBlockEcho);
  echo.detach();

  while(true) {
    int fd = accept(iSockfd, nullptr, nullptr);
    if(fd < 0) {
      exit(ERR_ACCEPT);
    }
    fcntl(fd, F_SETFL, O_NONBLOCK);
    fdQueue.push(fd);
  }
  return RUN_OK;
}
