#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <list>
#include <unordered_map>
#include "json/json.hpp"


#define DEFAULT_CONFIG_FILE "./etc/config.json"

#define d_cout std::cout << "[" << __FILE__ << " line: " << __LINE__ << " thread id: " << std::hex <<  pthread_self() << std::oct << "] "
#define MAX_CONNECTED_FDS_NUM 100000
#define EPOLL_MAX_EVENT_NUM  4096
#define EPOLL_WAIT_TIME 10000
#define MAX_BUF_SIZE 4096

#define SERVER_NAME "Http Server Debian Linux x64"

namespace Thread {
class Thread;
class MutexLock;
class CountDownLatch;
class Condition;
}

namespace Net {
class Channel;
class Epoll;
class Channel;
class Timer;
class TimerCompare;
class TimerManager;
class EventLoop;
class EventLoopThread;
class EventLoopThreadPool;

enum class HttpRecvState;
enum class HttpConnectionState;
enum class HttpParseURIResult;
enum class HttpParseHeaderResult;
enum class HttpResponseCode;
class HttpContentType;
class Http;

using SPHttp = std::shared_ptr<Net::Http>;
using SPEpoll = std::shared_ptr<Net::Epoll>;
using SPTimer = std::shared_ptr<Net::Timer>;
using SPTimerManager = std::shared_ptr<Net::TimerManager>;
using SPEventLoopThread = std::shared_ptr<Net::EventLoopThread>;
using SPChannel = std::shared_ptr<Net::Channel>;
}

namespace Process {
class Center;
enum class ResponseCode;
enum class Platform;
}

namespace Util {
using CallBack = std::function<void()>;
using CallBack1 = std::function<void(const std::string &)>;
using CallBack2 = std::function<void(const std::string &, const std::string &)>;
class Vessel;
}


namespace Third {
using Json = nlohmann::json;
}

namespace Data {
extern std::string root_path;
extern std::string web_page;
extern std::string web_404_page;
}

namespace Lgx {
class StartUp;
}




