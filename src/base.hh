#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>
#include "third/json.hh"

#define DEFAULT_CONFIG_FILE "./etc/config.json"

#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define MAX_CONNECTED_FDS_NUM 100000
#define EPOLL_MAX_EVENT_NUM  4096
#define EPOLL_WAIT_TIME 10000
#define MAX_BUF_SIZE 4096

#define SERVER_NAME "lgx-linux-1.2"

namespace lgx {
namespace thread {
class noncopyable;
class thread;
class thread_data;
class mutex_lock;
class mutex_lock_guard;
class count_down_latch;
class condition;
namespace current_thread {}

}
// namespace net start
namespace net {
class net;
class channel;
class epoll;
class channel;
class timer;
class timer_compare;
class timer_manager;
class eventloop;
class eventloop_thread;
class eventloop_threadpool;

enum class HttpRecvState;
enum class HttpConnectionState;
enum class HttpParseURIResult;
enum class HttpParseHeaderResult;
enum class HttpResponseCode;

class http_content_type;
class http;

using sp_http = std::shared_ptr<lgx::net::http>;
using sp_epoll = std::shared_ptr<lgx::net::epoll>;
using sp_timer = std::shared_ptr<lgx::net::timer>;
using sp_timer_manager = std::shared_ptr<lgx::net::timer_manager>;
using sp_eventloop_thread = std::shared_ptr<lgx::net::eventloop_thread>;
using sp_channel = std::shared_ptr<lgx::net::channel>;
}
// namespace net end


// namespace work start
namespace work {
class work;
enum class ResponseCode;
}
// namespace work end


// namespace util start
namespace util {
using callback = std::function<void()>;
using callback1 = std::function<void(const std::string &)>;
using callback2 = std::function<void(const std::string &, const std::string &)>;
class vessel;
}
// namespace util end


// namespace third start
namespace third {
using json = nlohmann::json;
}
// namespace third end


// namespace data start
namespace data {
extern std::string root_path;
extern std::string web_page;
extern std::string web_404_page;
extern std::map<std::string, std::string> firewall;
}
// namespace data end


class start_up;
}
