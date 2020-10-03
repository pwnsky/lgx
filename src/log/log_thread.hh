#pragma once

#include "../thread/thread.hh"
#include "../thread/mutex_lock.hh"
#include "../thread/condition.hh"
#include "log.hh"

class lgx::log::log_thread {
public:
    log_thread();
    ~log_thread();
    log *creat();
    void stop();
    void set_log_path(const std::string &log_path) {
        log_path_ = log_path;
    }
private:
    log *log_;
    bool exiting_;
    lgx::thread::thread thread_;
    lgx::thread::mutex_lock mutex_lock_;
    lgx::thread::condition condition_;
    void thread_func();
    std::string log_path_;
};
