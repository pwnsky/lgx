#pragma once
#include <memory>
#include <unordered_map>
#include <map>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#include "../base.hh"
#include "../net/eventloop.hh"
#include "../net/timer.hh"
#include "../work/work.hh"
#include "../util/vessel.hh"
#include "../log/log.hh"
#include "../security/firewall.hh"

using logger = lgx::log::logger;

enum class lgx::net::HttpRecvState {
    PARSE_HEADER = 0,
    RECV_CONTENT,
    WORK,
    FINISH
};

enum class lgx::net::HttpConnectionState {
    CONNECTED = 0,
    DISCONNECTING,
    DISCONNECTED
};

enum class lgx::net::HttpParseURIResult {
    SUCCESS = 0,
    ERROR
};

enum class lgx::net::HttpParseHeaderResult {
    SUCCESS = 0,
    ERROR
};

enum class lgx::net::HttpResponseCode {
    OK = 200,
    CREATED,
    ACCEPTED,
    NON_AUTHORITATIVE_INFORMATION,
    NO_CONTENT,
    RESET_CONTENT,
    PARTIAL_CONTENT,

    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    NOT_MODIFIED,
    USE_PROXY,
    SWITCH_PROXY,
    TEMPORARY_REDIRCT,
    RESUME_INCOMPLETE,

    BAD_REQUEST = 400,
    UNAUTHORIZED,
    PAYMENT_REAUIRED,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    PROXY_AUTHENTICATION_REQUIRED,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PRECONDITION_FAILED,
    REQUEST_ENTITY_TOO_LARGE,
    REQUEST_URI_TOO_LONG,
    UNSUPPORTED_MEDIA_TYPE,
    REQUESTED_RANGE_NOT_SATISFIABLE,
    EXPECTATION_FAILED,

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT,
    HTTP_VERSION_NOT_SUPPORTED,
    NETWORK_AUTHENTICATION_REQUIRED
};

class lgx::net::http_content_type final {
public:
    static std::string get_type(const std::string name);
private:
    static std::unordered_map<std::string, std::string> umap_type_;
    static pthread_once_t once_control_;
    static void init();
};

class lgx::net::http final : public std::enable_shared_from_this<http> {
public:
    explicit http(int fd,eventloop *elp);
    ~http();
    void reset();
    void unbind_timer();
    void bind_timer(sp_timer spt);
    sp_channel get_sp_channel();
    eventloop *get_eventloop();
    void handle_close();
    void new_evnet();
    void set_client_info(const std::string &ip, const std::string &port) {
        map_client_info_["client_ip"] = ip;
        map_client_info_["client_port"] = port;
        client_ip_ = ip;
        client_port_ = port;
    }
private:
    int fd_;
    eventloop *eventloop_;
    sp_channel sp_channel_;
    std::string header_data_;
    lgx::util::vessel in_buffer_;
    lgx::util::vessel out_buffer_;
    //std::string in_content_buffer_;
    bool recv_error_;
    HttpConnectionState http_connection_state_;
    HttpRecvState http_process_state_;
    int content_length_;

    bool keep_alive_;
    std::map<std::string, std::string> map_header_info_;
    std::weak_ptr<timer> wp_timer_;
    std::string client_ip_;
    std::string client_port_;
    std::map<std::string, std::string> map_client_info_;
    size_t error_times_ = 0; // for avoid attack
    size_t not_found_times_ = 0;

    void handle_read();
    void handle_write();
    void handle_connect();
    void handle_error(int error_number, std::string message);
    void handle_not_found();
    HttpParseHeaderResult parse_header();

    void handle_work();
    std::string get_suffix(std::string file_name);
    void send_data(const std::string &type,const std::string &content);
    void send_file(const std::string &file_name);
    bool check_file_path(const std::string &file_name);
    void str_lower(std::string &str);
    void redirect(const std::string &url);
};
