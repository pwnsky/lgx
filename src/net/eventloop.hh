#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "../thread/thread.hh"
#include "epoll.hh"
#include "util.hh"
#include <sys/epoll.h>
#include <sys/eventfd.h>

class lgx::net::eventloop {
public:
    eventloop();
    ~eventloop() {};
    int create_event_fd();
    void loop();
    void quit();
    void run_in_loop(lgx::util::callback &&func);
    void push_back(lgx::util::callback &&func);
    bool is_in_loop_thread();              // 判断是否在事件循环的线程中
    void assert_in_loop_thread();          // 在线程中断言
    void shutdown(sp_channel spc);                        // 关闭fd的写端
    void remove_from_epoll(sp_channel spc);                 // 移除事件
    void update_epoll(sp_channel spc, int ms_timeout = 0); // 更新epoll事件
    void add_to_epoll(sp_channel spc, int ms_timeout = 0);  // 添加epoll事件
private:
    bool looping_;
    int awake_fd_;
    bool quit_;
    bool event_handling_;
    const pid_t thread_id_;
    sp_epoll sp_epoll_;
    sp_channel sp_awake_channel_;           // 用于唤醒的Channel
    mutable lgx::thread::mutex_lock mutex_lock_; // 互斥锁
    std::vector<lgx::util::callback> pending_callback_functions_;
    bool calling_pending_callback_function_;

    void wake_up(); //for write one byte to client
    void run_pending_callback_func();
    void handle_read();
    void handle_connect();
};
