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
#include "log/log_thread.hh"
#include "log/log.hh"

#include "util/util.hh"
#include "security/firewall.hh"

using logger = lgx::log::logger;

class lgx::start_up final{
public:
     start_up();
    ~start_up();
    bool run();
    bool stop();
    bool load_config();
    bool run_logger_module();
    bool run_security_module();
    bool run_network_module();
    void show_logo();
#ifdef USE_DB_MYSQL
    bool connect_db_mysql();
#endif

private:
    int number_of_thread_;
    int queue_size_;
    int port_;
    std::string log_path_;
    std::shared_ptr<lgx::log::log_thread> sp_log_thread_;
    lgx::net::net net_;
};
