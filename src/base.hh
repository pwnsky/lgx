#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>
#include <queue>
#include "third/json.hh"

//#define DEBUG
#define DEFAULT_CONFIG_FILE "./etc/config.json"
#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define dbg_log(x) d_cout << x << std::endl

#define log_dbg(x) "LGX DEBUG:" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "\n" + std::string(x) + "\n"

#define MAX_CONNECTED_FDS_NUM 0x100000
#define EPOLL_MAX_EVENT_NUM   0x1000
#define EPOLL_WAIT_TIME       0x1000
#define MAX_BUF_SIZE          0x1000
#define HTTP_MAX_NOT_FOUND_TIMES 25
#define MAX_HTTP_RECV_BUF_SIZE 0x4000
#define LGX_VERSION "1.7"
#define SERVER_NAME "lgx " LGX_VERSION

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
using sp_eventloop = std::shared_ptr<lgx::net::eventloop>;
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
std::string date_time();
}
// namespace util end

// namespace log start
namespace log {
class io;
class log;
class logger;
class log_thread;
}
// namespace log end

// namespace third start
namespace third {
using json = nlohmann::json;
}
// namespace third end
namespace crypto {
class url;
}

namespace security {
class firewall;
}

namespace db {
namespace mysql {
class sql;
class query;
class exception;
}

namespace sqlite {
class sql;
class query;
class exception;
}
}


// namespace data start
namespace data {
extern std::string root_path;
extern std::string web_page;
extern std::string web_404_page;
extern std::string log_path;
extern lgx::log::log *p_log;
extern std::vector<std::string> forbid_ips;
extern lgx::security::firewall *firewall;
namespace mysql {
    extern lgx::db::mysql::sql *sql;
    extern lgx::thread::mutex_lock lock;
}

namespace sqlite {
    extern lgx::db::sqlite::sql *sql;
    extern lgx::thread::mutex_lock lock;
}

}
// namespace data end

class start_up;
}
