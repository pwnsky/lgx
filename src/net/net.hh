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
#include <openssl/ssl.h>
#include <openssl/err.h>


#include "base.hh"
#include "channel.hh"
#include "eventloop_threadpool.hh"
#include "util.hh"
#include "http.hh"
#include "log/log.hh"
#include "util/firewall.hh"

using logger = lgx::log::logger;

class lgx::net::net final{
public:
    net();
    net(int port, int number_of_thread);
    ~net();
    void init(int port, int number_of_thread);
    bool https_init(const std::string &https_crt, const std::string &https_key);
    void start();
    void stop();
    void handle_new_connection();
    void handle_reset();
private:
    int listen();    // Bind port_ and listen
    bool started_;   // Store state of net if started
    bool listened_;  // Store state of net if listend
    bool is_https_ = false;
    int port_;       // Listen port
    int number_of_thread_; // The number of thread
    eventloop *base_eventloop_ = nullptr;
    int listen_fd;
    sp_channel accept_channel_;
    std::unique_ptr<eventloop_threadpool> up_eventloop_threadpool_;
    int accept_fd_sum = 0;

    SSL_METHOD *ssl_method_ = nullptr;
    SSL_CTX *ssl_ctx_ = nullptr;
    SSL *ssl_ = nullptr;
};

