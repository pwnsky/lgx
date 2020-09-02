#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <iostream>
#include <cassert>
#include <string.h>

#include "channel.hh"
#include "timer.hh"
#include "../base.hh"

class lgx::net::epoll {
public:
    epoll();
    ~epoll();
    void add(sp_channel sp_channel, int ms_timeout);
    void mod(sp_channel sp_channel, int ms_timeout);
    void del(sp_channel sp_channel);
    std::vector<sp_channel> get_all_event_channels();
    std::vector<sp_channel> get_event_channels_after_get_events(int number_of_events);
    void add_timer(sp_channel sp_channel, int ms_timeout);
    int get_epoll_fd();
    void handle_expired_event();
private:
    int epoll_fd_;
    std::vector<epoll_event> v_events_;
    sp_channel sp_channels_[MAX_CONNECTED_FDS_NUM];
    sp_http sp_https_[MAX_CONNECTED_FDS_NUM]; // It's not https protocol
    sp_timer_manager sp_timer_manager_;
};



