TARGET = lgx
CONFIG += c++11

HEADERS += \
    base.hh \
    startup.hh \
    \
    json/json.hpp\
    process/center.hh \
    \
    net/http.hh \
    net/epoll.hh \
    net/net.hh \
    net/util.hh \
    net/channel.hh \
    net/eventloop.hh \
    net/eventloop_thread.hh \
    net/eventloop_threadpool.hh \
    net/timer.hh \
    net/vessel.hpp \
    \
    thread/noncopyable.hh \
    thread/thread.hh \
    thread/mutex_lock.hh \
    thread/condition.hh \
    thread/count_down_latch.hh \

SOURCES += \
    main.cc \
    startup.cc \
    \
    process/center.cc \
    \
    net/http.cc \
    net/epoll.cc \
    net/net.cc \
    net/util.cc \
    net/channel.cc \
    net/eventloop.cc \
    net/eventloop_thread.cc \
    net/eventloop_threadpool.cc \
    net/timer.cc \
    \
    thread/thread.cc \
    thread/mutex_lock.cc \
    thread/condition.cc \
    thread/count_down_latch.cc \





