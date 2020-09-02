#pragma once

#include "../base.hh"
#include "eventloop.hh"
#include "../thread/thread.hh"
#include "../thread/mutex_lock.hh"
#include "../thread/condition.hh"

class lgx::net::eventloop_thread {
public:
     explicit eventloop_thread();
    ~eventloop_thread();
    eventloop *start_loop();

private:
    eventloop *eventloop_;
    bool exiting_;
    lgx::thread::thread thread_;
    lgx::thread::mutex_lock mutex_lock_;
    lgx::thread::condition condition_;
    void thread_func();
};

