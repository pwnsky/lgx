#include "log_thread.hh"

lgx::log::log_thread::log_thread() :
    log_(nullptr),
    exiting_(false),
    thread_(std::bind(&log_thread::thread_func, this), "log eventloop thread!"),
    mutex_lock_(),
    condition_(mutex_lock_) {
}

lgx::log::log_thread::~log_thread() {
    exiting_ = true;
    if(log_ != nullptr) {
        log_->quit();
        thread_.join();
    }
}

lgx::log::log *lgx::log::log_thread::creat() {
    assert(thread_.is_started() == false);
    thread_.start();

    //Waiting for run
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_);
    while(log_ == nullptr)
        condition_.wait();
    // return a new eventloop object ptr
    return log_;
}

void lgx::log::log_thread::thread_func() {
    // create a new eventloop
    log l;
    log_ = &l;
    log_->set_log_path(log_path_); // set the path to log
    condition_.signal();    // Notify Main thread then realize Sync
    log_->loop();      //run event
    log_ = nullptr;
}

void lgx::log::log_thread::stop() {
     log_->quit();
     lgx::data::p_log = nullptr;
}

