#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sys/epoll.h>

#include "eventloop.hh"
#include "http.hh"
#include "../base.hh"


class lgx::net::channel {
public:
    channel(eventloop *elp);
    channel(eventloop *elp, int fd);
    ~channel();
    void set_fd(int fd);
    int get_fd();
    lgx::net::sp_http get_holder();
    void set_holder(sp_http  sp_http);

    void handle_read();
    void handle_write();
    void handle_connect(); // handle new connect
    void handle_event();
    void handle_error();
    void set_revent(__uint32_t revent);
    void set_event(__uint32_t event);
    void set_read_handler (lgx::util::callback &&read_handler);
    void set_write_handler(lgx::util::callback &&write_handler);
    void set_error_handler(lgx::util::callback &&error_handler);
    void set_connected_handler(lgx::util::callback &&connected_handler);
    __uint32_t &get_event();
    __uint32_t get_last_event();
    void update_last_evnet();
    bool is_last_event();

private:
    eventloop *elp_ = nullptr;
    int fd_ = -1;
    __uint32_t event_ = 0;
    __uint32_t revent_ = 0;
    __uint32_t last_event_ = 0;
    std::weak_ptr<lgx::net::http> holder_;
    lgx::util::callback read_handler_ = nullptr;
    lgx::util::callback write_handler_ = nullptr;
    lgx::util::callback error_handler_ = nullptr;
    lgx::util::callback connected_handler_ = nullptr;
};
