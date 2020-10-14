#include "log.hh"

lgx::log::log *lgx::data::p_log;
std::string lgx::data::log_path;


lgx::log::io::io() : nums_(0){

}

lgx::log::io::~io() {
}


void lgx::log::io::open(const std::string &log_path) {
    log_fd_ = ::open(log_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0755);
    //std::cout << "log file: " << lgx::data::log_path.c_str() << '\n';
    if(-1 == log_fd_) {
        perror("open log file");
        abort();
    }
}

void lgx::log::io::close() {
    ::close(log_fd_);
}

void lgx::log::io::write() {
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_);
    if(logs_.size() > 0) {
        std::string log = logs_.front();
        logs_.pop();
        ::write(log_fd_, log.data(), log.size());
    }
    nums_ --;
}

void lgx::log::io::push(const std::string &log) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_);
    logs_.push(log);
    nums_ ++;
}


lgx::log::log::log() : quit_(false) {
    quit_ = false;
}

lgx::log::log::~log() {

}
void lgx::log::log::loop() {
    io_.open(log_path_);
    while (!quit_) {
        if(io_.nums_ > 0)
            io_.write();
    }
    std::cout << "stop log module\n";
    io_.close();
}

void lgx::log::log::quit() {
    quit_ = true;
}

