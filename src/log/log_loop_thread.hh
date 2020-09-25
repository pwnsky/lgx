#pragma once

#include "../thread/thread.hh"
#include "../thread/mutex_lock.hh"
#include "../thread/condition.hh"

#include "log_loop.hh"

class lgx::log::log_loop_thread {
public:
    log_loop_thread();
    ~log_loop_thread();
    log_loop *start_loop();
private:
    log_loop *log_loop_;
    bool exiting_;
    lgx::thread::thread thread_;
    lgx::thread::mutex_lock mutex_lock_;
    lgx::thread::condition condition_;
    void thread_func();
};
