#pragma once

#include <memory>
#include <queue>
#include <deque>
#include <sys/time.h>
#include <unistd.h>

#include "../base.hh"
#include "http.hh"

class lgx::net::timer {
public:
    timer(sp_http sph, int ms_timeout);
    timer(timer &t);
    ~timer();
    void update(int ms_timeout);
    bool is_valid();
    void clear();
    void set_deleted();
    bool is_deleted();
    time_t get_expired_time();
private:
    bool deleted_;
    time_t expired_ms_time_;
    sp_http sp_http_;
};

class lgx::net::timer_compare {
public:
    bool operator()(sp_timer &sp_timer_1, sp_timer &sp_timer_2) const {
        return sp_timer_1->get_expired_time() > sp_timer_2->get_expired_time();
    }
};

class lgx::net::timer_manager {
public:
    explicit timer_manager() {};
    ~timer_manager() {};
    void add_timer(sp_http sp_http, int ms_timeout);
    void handle_expired_event();
private:
    std::priority_queue<sp_timer, std::deque<sp_timer>, timer_compare>
    sort_sp_timer_queue;
};

