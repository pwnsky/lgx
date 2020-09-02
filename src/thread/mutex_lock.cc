#include "mutex_lock.hh"

lgx::thread::mutex_lock::mutex_lock() {
    pthread_mutex_init(&mutex_, nullptr);
}

lgx::thread::mutex_lock::~mutex_lock() {
    // 线程调用该函数让互斥锁上锁，如果该互斥锁已被另一个线程锁定和拥有，则调用该线程将阻塞，直到该互斥锁变为可用为止
    pthread_mutex_lock(&mutex_);
    // 销毁互斥锁
    pthread_mutex_destroy(&mutex_);
}

void lgx::thread::mutex_lock::lock() {
    pthread_mutex_lock(&mutex_);
}

void lgx::thread::mutex_lock::unlock() {
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t *lgx::thread::mutex_lock::get_mutex() {
    return &mutex_;
}


lgx::thread::mutex_lock_guard::mutex_lock_guard(lgx::thread::mutex_lock &mutex_lock) : mutex_lock_(mutex_lock){
    mutex_lock_.lock();
}
lgx::thread::mutex_lock_guard::~mutex_lock_guard(){
    mutex_lock_.unlock();
}
