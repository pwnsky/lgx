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

class lgx::net::net final{
public:
    net(int port,int thread_number);
    ~net() {};
    void start();
    void handle_new_connection();
    void handle_connected();
private:
    int listen();    // Bind port_ and listen
    bool started_;   // Store state of net if started
    bool listened_;  // Store state of net if listend
    int port_;       // Listen port
    int number_of_thread_; // The number of thread
    eventloop *base_eventloop_;
    int listen_fd;
    sp_channel accept_channel_;
    std::unique_ptr<eventloop_threadpool> up_eventloop_threadpool_;
    int accept_fd_sum = 0;
};

