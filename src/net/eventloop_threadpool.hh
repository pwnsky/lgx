#pragma once

#include "../base.hh"
#include "eventloop.hh"
#include "eventloop_thread.hh"

class lgx::net::eventloop_threadpool {
public:
    eventloop_threadpool(eventloop *base_eventloop, int number_of_thread);
    ~eventloop_threadpool() {};
    void start();
    eventloop *get_next_eventloop();
private:
    bool started_;
    eventloop *base_eventloop_;
    int number_of_thread_;
    int next_thread_indx_;
    std::vector<eventloop *> v_eventloops_;
    std::vector<sp_eventloop_thread> v_sp_eventloop_threads_;
};
