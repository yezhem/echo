#include "common.h"

#include <vector>

#include <fcntl.h>
#include <sys/select.h>

const char* program  = __FILE__;

int run(int iSockfd) {
  char buffer[MAX_BUFFER];
  int maxSockfd = iSockfd;
  std::vector<int> fds;

  fcntl(iSockfd, F_SETFL, O_NONBLOCK);

  fd_set rset;
  FD_ZERO(&rset);
  FD_SET(iSockfd, &rset);

  while(true) {
    fd_set set = rset;
    select(maxSockfd + 1, &set, nullptr, nullptr, nullptr);

    for(auto it = fds.begin(); it != fds.end(); ) {
      if(FD_ISSET(*it, &set)) { 
        ssize_t sRecv = recv(*it, buffer, sizeof(buffer), 0);
        if(sRecv < 0) {
          exit(ERR_RECV);
        }else if(sRecv == 0) {
          close(*it);
          FD_CLR(*it, &rset);
          it = fds.erase(it);
        }else {
          send(*it, buffer, sRecv, 0);
          ++it;
        }
      }else { ++it; }
    }

    if(FD_ISSET(iSockfd, &set)) {
      int fd = accept(iSockfd, nullptr, nullptr);
      if(fd < 0 && errno != EAGAIN) {
        exit(ERR_ACCEPT);
      } else if(fd > 0) {
        fcntl(fd, F_SETFL, O_NONBLOCK);
        fds.push_back(fd);
        FD_SET(fd, &rset);
        maxSockfd = maxSockfd > fd ? maxSockfd : fd;
      }
    }
  }
  return RUN_OK;
}
