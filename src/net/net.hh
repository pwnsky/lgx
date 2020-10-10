#pragma once
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <errno.h>
#include <cstring>

#include "../base.hh"
#include "channel.hh"
#include "eventloop_threadpool.hh"
#include "util.hh"
#include "http.hh"
#include "../log/log.hh"
#include "../security/firewall.hh"

using logger = lgx::log::logger;

class lgx::net::net final{
public:
    net();
    net(int port, int number_of_thread);
    ~net();
    void set_port(int port);
    void set_number_of_thread(int number_of_thread);
    void start();
    void stop();
    void handle_new_connection();
    void handle_connected();
private:
    int listen();    // Bind port_ and listen
    bool started_;   // Store state of net if started
    bool listened_;  // Store state of net if listend
    int port_;       // Listen port
    int number_of_thread_; // The number of thread
    eventloop *base_eventloop_ = nullptr;
    int listen_fd;
    sp_channel accept_channel_;
    std::unique_ptr<eventloop_threadpool> up_eventloop_threadpool_;
    int accept_fd_sum = 0;
};

