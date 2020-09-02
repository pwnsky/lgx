#include "eventloop_threadpool.hh"


lgx::net::eventloop_threadpool::eventloop_threadpool(eventloop *base_eventloop, int number_of_thread) :
    started_(false),
    base_eventloop_(base_eventloop),
    number_of_thread_(number_of_thread),
    next_thread_indx_(0) {
    if(number_of_thread <= 0) {
        d_cout << "The number of thread must be >= 1\n";
        abort();
    }
}

lgx::net::eventloop *lgx::net::eventloop_threadpool::get_next_eventloop() {
    base_eventloop_->assert_in_loop_thread();
    assert(started_);
    eventloop *eventloop = base_eventloop_;
    if(v_eventloops_.empty() == false) {
        eventloop = v_eventloops_[next_thread_indx_];
        next_thread_indx_ = (next_thread_indx_ + 1) % (number_of_thread_);
    }
    return eventloop;
}

void lgx::net::eventloop_threadpool::start() {
    base_eventloop_->assert_in_loop_thread();
    started_ = true;
    for(int idx = 0; idx < number_of_thread_; ++idx) {
        sp_eventloop_thread sp_elt(new eventloop_thread());
        v_sp_eventloop_threads_.push_back(sp_elt);
        //store a new eventloop
        v_eventloops_.push_back(sp_elt->start_loop());
    }
}
