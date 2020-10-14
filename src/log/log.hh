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
#include "../thread/mutex_lock.hh"
#include <atomic>
#include <sys/sem.h>
#include <semaphore.h>

extern lgx::log::log *lgx::data::p_log;
extern std::string lgx::data::log_path;

#include <atomic>
// handling in log eventloop thread
class lgx::log::io {
public:
    io();
    ~io();
    void close();
    void open(const std::string &log_path);
    void write();
    void push(const std::string &log);
    std::atomic<int> nums_;
private:
    int log_fd_ = -1;
    std::queue<std::string> logs_;
    lgx::thread::mutex_lock mutex_lock_;


};

class lgx::log::log {
public:
    log();
    ~log();
    void loop();
    void quit();
    void set_log_path(const std::string &log_path) {
        log_path_ = log_path;
    }
    void push(const std::string &log) {
        io_.push(log);
    }

private:
    std::atomic<bool> quit_;
    io io_;
    std::string log_path_;
};

class lgx::log::logger {
public:
    void operator<< (const std::string &t) {
        if(lgx::data::p_log)
            lgx::data::p_log->push('\n' + lgx::util::date_time() + '\n' + t);
    }
private:

};
