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

class lgx::start_up final{
public:
     start_up();
    ~start_up();
    bool run();
    bool stop();
    bool load_config();
    bool run_logger_module();
    bool run_network_module();
    void show_logo();

private:
    int number_of_thread_;
    int queue_size_;
    int port_;
    std::string log_path_;
};
