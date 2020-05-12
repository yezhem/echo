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

constexpr int MAX_BUFFER = 4096;
constexpr int BACKLOG    = 5;

int run(int);
int doEcho(int fd);
