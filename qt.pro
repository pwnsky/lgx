 

HEADERS += \
    src/base.hh \
    src/third/json.hh \
    src/util/vessel.hh \
    src/util/util.hh \
    src/log/log.hh \
    src/log/log_loop.hh \
    src/log/log_loop_thread.hh \
    src/net/channel.hh \
    src/net/epoll.hh \
    src/net/eventloop.hh \
    src/net/eventloop_thread.hh \
    src/net/eventloop_threadpool.hh \
    src/net/http.hh \
    src/net/net.hh \
    src/net/timer.hh \
    src/net/util.hh \
    src/startup.hh \
    src/thread/condition.hh \
    src/thread/count_down_latch.hh \
    src/thread/mutex_lock.hh \
    src/thread/noncopyable.hh \
    src/thread/thread.hh \
    src/work/work.hh \
    src/crypto/url.hh

SOURCES += \
    src/log/log.cc \
    src/log/log_loop.cc \
    src/log/log_loop_thread.cc \
    src/util/util.cc \
    src/main.cc \
    src/net/channel.cc \
    src/net/epoll.cc \
    src/net/eventloop.cc \
    src/net/eventloop_thread.cc \
    src/net/eventloop_threadpool.cc \
    src/net/http.cc \
    src/net/net.cc \
    src/net/timer.cc \
    src/net/util.cc \
    src/startup.cc \
    src/thread/thread.cc \
    src/work/work.cc
