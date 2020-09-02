#pragma once
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "../base.hh"
#include "noncopyable.hh"
#include "mutex_lock.hh"

class lgx::thread::condition : noncopyable {
  public:
    explicit condition(mutex_lock &ml);
    ~condition();
    void wait();
    void signal();
    void broadcast();
    bool wait_for_seconds(int seconds);
private:
    mutex_lock &mutex_;
    pthread_cond_t cond_ = PTHREAD_COND_INITIALIZER;
};

