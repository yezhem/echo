#include "common.h"

#include <fcntl.h>

#include <vector>

const char* program  = __FILE__;

// nonblock - 非阻塞模式
int run(int iSockfd) {
  std::vector<int> fdList;
  char buffer[MAX_BUFFER];

  fcntl(iSockfd, F_SETFL, O_NONBLOCK);

  while(true) {
    int fd = accept(iSockfd, nullptr, nullptr);
    if(fd < 0 && errno != EAGAIN) {
      exit(ERR_ACCEPT);
    } else if(fd > 0) {
      fcntl(fd, F_SETFL, O_NONBLOCK);
      fdList.push_back(fd);
    }
    // nonblock echo
    for(auto it = fdList.begin(); it != fdList.end(); ) {
      ssize_t sRecv = recv(*it, buffer, sizeof(buffer), 0);
      if(sRecv == 0) {
        close(*it);
        it = fdList.erase(it);
      }else if(sRecv < 0 && errno != EAGAIN) {
        std::cerr << "error to recv" << std::endl;
        it = fdList.erase(it);
      }else if(sRecv < 0) {
        ++it;
      }else {
        send(*it, buffer, sRecv, 0);
        ++it;
      }
    }
  }

  return RUN_OK;
}
