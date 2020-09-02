#include "eventloop_thread.hh"

lgx::net::eventloop_thread::eventloop_thread() :
    eventloop_(nullptr),
    exiting_(false),
    thread_(std::bind(&eventloop_thread::thread_func, this), "eventloop_thread"),
    mutex_lock_(),
    condition_(mutex_lock_) {

}

lgx::net::eventloop_thread::eventloop_thread::~eventloop_thread() {
    exiting_ = true;
    if(eventloop_ != nullptr) {
        eventloop_->quit();
        thread_.join();
    }
}
lgx::net::eventloop *lgx::net::eventloop_thread::eventloop_thread::start_loop() {
    assert(thread_.is_started() == false);
    thread_.start();

    //Waiting for run
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_);
    while(eventloop_ == nullptr)
        condition_.wait();

    // return a new eventloop object ptr
    return eventloop_;
}
void lgx::net::eventloop_thread::eventloop_thread::thread_func() {
    // create a new eventloop
    eventloop el;
    eventloop_ = &el;
    condition_.signal(); // Notify Main thread then realize Sync
    eventloop_->loop();  //run event
    eventloop_ = nullptr;
}
