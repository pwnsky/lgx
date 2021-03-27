#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>
#include <queue>
#include "util/json.hh"

//#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << "] "
#define dbg_log(x) d_cout << x << std::endl
#define log_dbg(x) "LGX DEBUG:" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "\n" + std::string(x) + "\n"

//#define DEFAULT_CONFIG_FILE "/etc/lgx/conf.json"
//#define DEBUG

#define MAX_CONNECTED_FDS_NUM 0x100000
#define EPOLL_MAX_EVENT_NUM   0x1000
#define EPOLL_WAIT_TIME       0x1000
#define MAX_BUF_SIZE          0x1000
#define HTTP_MAX_NOT_FOUND_TIMES 25
#define MAX_HTTP_RECV_BUF_SIZE 0x4000
#define LGX_VERSION "1.9"
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
class chat;
}
// namespace work end

// namespace util start
namespace util {
using callback = std::function<void()>;
using callback1 = std::function<void(const std::string &)>;
using callback2 = std::function<void(const std::string &, const std::string &)>;
class vessel;
using json = nlohmann::json;
std::string date_time();
class url;
class firewall;
class md5;
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


// namecpce of chat
namespace chat {
enum class request_type;
struct msg {
    enum class type {
        USER,
        GROUP,
        System
    };
    msg::type type;
    lgx::util::json content;
    std::string tid;
    std::string uid;
    time_t time;
};

struct user{
public:
    std::string uid;
    std::string name;
    std::string client_session;
};

struct group {
public:
    std::string holder_uid;
    std::string gid;
    std::string name;
    std::list<std::string> member_uid_list;
};
}
// namecpce of chat end



// namespace data start
namespace data {
extern std::string root_path;
extern std::string web_page;
extern std::string web_404_page;
extern std::string log_path;
extern lgx::log::log *p_log;
extern std::vector<std::string> forbid_ips;
extern lgx::util::firewall *firewall;
extern std::string config_path;
extern std::string os_info;
//namespace mysql {
//    extern lgx::db::mysql::sql *sql;
//    extern lgx::thread::mutex_lock lock;
//}
extern std::map<std::string, std::weak_ptr<lgx::net::http>> sessions;

extern std::unordered_map<std::string, lgx::chat::user> users;
extern std::unordered_map<std::string, lgx::chat::group> groups;

}
// namespace data end
class start_up;
}
