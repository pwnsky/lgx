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

void lgx::net::channel::handle_connect() {
    if(connected_handler_) {
        connected_handler_();
    }
}
void lgx::net::channel::handle_error() {
    if(error_handler_) {
        error_handler_();
    }
}

void lgx::net::channel::handle_event() {
    event_ = 0;
    if((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {
        event_ = 0;
        return;
    }
    if(revent_ & EPOLLERR) {
        if(error_handler_) handle_error();
        event_ = 0;
        return ;
    }
    if(revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        handle_read();
    }
    if(revent_ & EPOLLOUT) {
        handle_write();
    }
    //*******
    handle_connect();
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
void lgx::net::channel::set_connected_handler(lgx::util::callback  &&connected_handler) {
    connected_handler_ = connected_handler;
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
