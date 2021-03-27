# server src headers
HEADERS += \
    src/base.hh \
    src/util/json.hh \
    src/util/vessel.hh \
    src/util/util.hh \
    src/log/log.hh \
    src/log/log_thread.hh \
    src/net/channel.hh \
    src/net/epoll.hh \
    src/net/eventloop.hh \
    src/net/eventloop_thread.hh \
    src/net/eventloop_threadpool.hh \
    src/net/http.hh \
    src/net/net.hh \
    src/net/timer.hh \
    src/net/util.hh \
    src/start_up.hh \
    src/thread/condition.hh \
    src/thread/count_down_latch.hh \
    src/thread/mutex_lock.hh \
    src/thread/noncopyable.hh \
    src/thread/thread.hh \
    src/work/work.hh \
    src/work/chat.hh \
    src/util/url.hh \
    src/util/firewall.hh \
    src/util/md5.hh \
    src/util/mysql/sql.hh \
    src/util/mysql/query.hh
# server src sources
SOURCES += \
    src/log/log.cc \
    src/log/log_thread.cc \
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
    src/start_up.cc \
    src/thread/thread.cc \
    src/work/work.cc \
    src/work/chat.cc \
    src/util/firewall.cc \
    src/util/md5.cc \
    src/util/mysql/sql.cc \
    src/util/mysql/query.cc


# test src headers
HEADERS += \


# test src sources
SOURCES += \
    test_src/lgx_chat.py \
    webbench.c
