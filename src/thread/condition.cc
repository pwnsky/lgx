#include "condition.hh"

lgx::thread::condition::condition(mutex_lock &ml) : mutex_(ml) {
    pthread_cond_init(&cond_, nullptr);
}
lgx::thread::condition::~condition() {
    pthread_cond_destroy(&cond_);
}

void lgx::thread::condition::wait() {
    pthread_cond_wait(&cond_, mutex_.get_mutex());

}

void lgx::thread::condition::signal() {
    pthread_cond_signal(&cond_);
}

void lgx::thread::condition::broadcast() {
    pthread_cond_broadcast(&cond_);
}

bool lgx::thread::condition::wait_for_seconds(int seconds) {
    struct timespec time_spec;
    clock_gettime(CLOCK_REALTIME, &time_spec);
    time_spec.tv_sec += static_cast<__time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get_mutex(), &time_spec);
}


