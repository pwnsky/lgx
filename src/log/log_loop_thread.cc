#include "log_loop_thread.hh"

lgx::log::log_loop_thread::log_loop_thread() :
    log_loop_(nullptr),
    exiting_(false),
    thread_(std::bind(&log_loop_thread::thread_func, this), "log eventloop thread!"),
    mutex_lock_(),
    condition_(mutex_lock_) {
}

lgx::log::log_loop_thread::~log_loop_thread() {
    exiting_ = true;
    if(log_loop_ != nullptr) {
        log_loop_->quit();
        thread_.join();
    }
}

lgx::log::log_loop *lgx::log::log_loop_thread::start_loop() {
    assert(thread_.is_started() == false);
    thread_.start();

    //Waiting for run
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_);
    while(log_loop_ == nullptr)
        condition_.wait();
    // return a new eventloop object ptr
    return log_loop_;
}

void lgx::log::log_loop_thread::thread_func() {
    // create a new eventloop
    log_loop l;
    log_loop_ = &l;
    condition_.signal();    // Notify Main thread then realize Sync
    log_loop_->loop();      //run event
    log_loop_ = nullptr;
}
void lgx::log::log_loop_thread::stop() {
     log_loop_->quit();
}

