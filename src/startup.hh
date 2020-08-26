#pragma once
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "net/net.hh"
#include "net/eventloop.hh"
#include "base.hh"

class Lgx::StartUp final{
public:
     StartUp();
    ~StartUp();
    bool Run();
    bool Stop();
    bool LoadConfig();
    bool RunLoggerModule();
    bool RunNetworkModule();

private:
    int number_of_thread_;
    int queue_size_;
    int port_;
    std::string log_path_;
};
