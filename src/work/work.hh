#pragma once

#include <map>
#include <string>
#include <unistd.h>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unordered_map>
#include <pthread.h>

#include "../base.hh"
#include "../third/json.hh"

enum class lgx::work::ResponseCode {
    SUCCESS = 0,
    FAILURE,
    NOT_FOUND,
    INTERNAL_ERROR,
    NO_ACCESS,
    NO_PERMISSSION = 5,
    ERROR_PARSING_URL,
    ERROR_PARSING_CONTENT,
    ERROR_HTTP_CONTENT,
    ERROR_JSON_CONTENT_TYPE,
    ERROR_JSON_CONTENT = 10,
    EXIST,
    NOT_EXIST,
    LOGINED,
};

class lgx::work::work {
    using json = lgx::third::json;
public:
    explicit work(const std::map<std::string, std::string> &map_header_info, std::string &content); // uid for deal with offline
    ~work() {};
    void set_send_data_handler(lgx::util::callback2 send_data_handler);
    void set_error_handler(lgx::util::callback2 error_handler);
    void set_send_file_handler(lgx::util::callback1 send_file_handler);
    void set_fd(int fd) ;

    void run();
    void handle_get();
    void handle_post();
    void handle_put();
private:
    const std::map<std::string, std::string> &map_header_info_;
    std::string &content_;
    lgx::util::callback1 send_file_handler_;
    lgx::util::callback2 send_data_handler_;
    lgx::util::callback2 error_handler_;
    std::map<std::string, std::string> map_url_info_;
    std::map<std::string, std::string> map_url_value_info_;
    std::string request_;
    std::string platform_;
    int fd_;
    void send_data(const std::string &suffix, const std::string &content);
    void send_json(lgx::third::json &json_obj);
    void send_file(std::string file_name);
    bool parse_url();
    void handle_not_found();
    void response(ResponseCode error_code);
    std::string get_date_time();
};
