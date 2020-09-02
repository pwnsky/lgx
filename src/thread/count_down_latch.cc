#include "count_down_latch.hh"

void lgx::thread::count_down_latch::wait() {
    mutex_lock_guard mlg(mutex_);
    while(count_ > 0) condition_.wait();
}

void lgx::thread::count_down_latch::count_down() {
    mutex_lock_guard mlg(mutex_);
    --count_;
    if(count_ <= 0)  condition_.broadcast();
}
