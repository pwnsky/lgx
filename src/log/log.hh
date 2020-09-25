#pragma once

#include "../base.hh"
#include <sstream>
#include <queue>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "../util/util.hh"

extern lgx::log::log_loop *lgx::data::log_loop;
extern int lgx::data::log_fd;
extern std::string lgx::data::log_path;
class lgx::log::log {
public:
    void operator<< (const std::string &t) {
        lgx::data::logs.push('\n' + lgx::util::date_time() + '\n' + t);
    }
};

// handling in log eventloop thread
class lgx::log::log_io {
public:
    void close();
    void open();
    void write();
};
