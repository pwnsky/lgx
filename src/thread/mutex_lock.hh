#pragma once
#include "noncopyable.hh"
#include <pthread.h>

class lgx::thread::mutex_lock : noncopyable {
public:
    mutex_lock();
    ~mutex_lock();
    void lock();
    void unlock();
    pthread_mutex_t *get_mutex();
private:
    pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
    //Friend classes are not affected by access rights
    friend class Condition;
};

class lgx::thread::mutex_lock_guard : lgx::thread::noncopyable {
public:
    explicit mutex_lock_guard(mutex_lock &ml);
    ~mutex_lock_guard();
private:
    mutex_lock &mutex_lock_;
};

