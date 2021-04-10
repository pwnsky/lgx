#include "log.hh"

lgx::log::log *lgx::data::p_log;
std::string lgx::data::log_path;


lgx::log::io::io(){
}

lgx::log::io::~io() {
}

bool lgx::log::io::open(const std::string &log_path) {
    log_fd_ = ::open(log_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0755);
    //std::cout << "log file: " << lgx::data::log_path.c_str() << '\n';
    if(-1 == log_fd_) {
        perror(("Open log file: " + log_path).c_str());
        return false;
    }
    return true;
}

void lgx::log::io::close() {
    ::close(log_fd_);
}

void lgx::log::io::write() {
    lgx::thread::mutex_lock_guard mutex_lock_guard(write_mutex_lock_);
    if(logs_.size() > 0) {
        std::string log = logs_.front();
        logs_.pop();
        ::write(log_fd_, log.data(), log.size());
    }
}

void lgx::log::io::push(const std::string &log) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(write_mutex_lock_);
    logs_.push(log);
}


lgx::log::log::log() : quit_(false) ,
    cond_(log_wait_){
    quit_ = false;
}

lgx::log::log::~log() {

}
void lgx::log::log::loop() {
    if(io_.open(log_path_) == false) {
        quit_ = true;
        cond_.broadcast();
        exit(-1);
        return;
    }

    while (!quit_) {
        cond_.wait();
        io_.write();
    }
    std::cout << "stop log module\n";
    io_.close();
}

void lgx::log::log::quit() {
    quit_ = true;
    cond_.broadcast();
}

