#include "channel.hh"

lgx::net::channel::channel(eventloop *elp) {
    elp_ = elp;
}

lgx::net::channel::channel(eventloop *elp, int fd) {
    elp_ = elp;
    fd_ = fd;
}

lgx::net::channel::~channel() {

}

void lgx::net::channel::set_fd(int fd) {
    fd_ = fd;
}

int lgx::net::channel::get_fd() {
    return fd_;
}

void lgx::net::channel::set_holder(sp_http sph) {
    holder_ = sph;
}

lgx::net::sp_http lgx::net::channel::get_holder() {
    sp_http ret = holder_.lock();
    return ret;
}

void lgx::net::channel::handle_read() {
    if(read_handler_) {
        read_handler_();
    }
}

void lgx::net::channel::handle_write() {
    if(write_handler_) {
        write_handler_();
    }
}

void lgx::net::channel::handle_reset() {
    if(reset_handler_) {
        reset_handler_();
    }
}
void lgx::net::channel::handle_error() {
    if(error_handler_) {
        error_handler_();
    }
}

void lgx::net::channel::handle_event() {
    event_ = 0; //处理后的事件清0
    if((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {
        event_ = 0;
        return;
    }
    // 处理错误
    if(revent_ & EPOLLERR) {
        if(error_handler_) handle_error();
        event_ = 0;
        return ;
    }

    // 优先处理有数据将要写入
    if(revent_ & EPOLLOUT) {
        handle_write();
    }

    // 有数据来临
    if(revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        handle_read();
    }

    //*******
    handle_reset();
}

void lgx::net::channel::set_revent(__uint32_t revent) {
    revent_ = revent;
}
void lgx::net::channel::set_event(__uint32_t event) {
    event_ = event;
}

void lgx::net::channel::set_read_handler(lgx::util::callback  &&read_handler) {
    read_handler_ = read_handler;
}

void lgx::net::channel::set_write_handler(lgx::util::callback  &&write_handler) {
    write_handler_ = write_handler;
}

void lgx::net::channel::set_error_handler(lgx::util::callback  &&error_handler) {
    error_handler_ = error_handler;
}

// For deal with connected client event
void lgx::net::channel::set_reset_handler(lgx::util::callback  &&reset_handler) {
    reset_handler_ = reset_handler;
}

__uint32_t &lgx::net::channel::get_event() {
    return event_;
}

__uint32_t lgx::net::channel::get_last_event() {
    return last_event_;
}

void lgx::net::channel::update_last_evnet() {
    last_event_ = event_;
}

bool lgx::net::channel::is_last_event() {
    return last_event_ == event_;
}
