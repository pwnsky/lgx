#pragma once
#include <cstdlib>
#include <string>
#include <string.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "../base.hh"
#include "../util/vessel.hh"

namespace lgx{
namespace net {
namespace util {
ssize_t read(int fd, void *buffer, size_t length);
ssize_t read(int fd, std::string &in_buffer);
ssize_t read(int fd, std::string &in_buffer, int length);

ssize_t write(int fd, void *buffer, size_t length);
ssize_t write(int fd, lgx::util::vessel &out_buffer);

void ignore_sigpipe();                 //avoid server terminate with SIGPIPE signal
bool set_fd_nonblocking(int listen_fd); //set fd as non bloking
void set_fd_nodelay(int fd);            //set fd no delay
void set_fd_nolinger(int fd);           //set fd no linger
void shutdown_write_fd(int fd);         //shutdown fd of write
bool wall(int fd, const std::string &ip);       // fire wall
}
}
}

