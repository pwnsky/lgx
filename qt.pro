 

HEADERS += \
    src/base.hh \
    src/third/json.hh \
    src/log/log.hh \
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
    src/util/vessel.hh \
    src/work/work.hh

SOURCES += \
    src/log/log.cc \
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
