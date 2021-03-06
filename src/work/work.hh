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
#include <iterator>

#include "base.hh"
#include "log/log.hh"
#include "util/url.hh"
#include "util/vessel.hh"
#include "net/http.hh"
#include "chat.hh"

using logger = lgx::log::logger;

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
    using json = lgx::util::json;
public:
    explicit work(const std::map<std::string, std::string> &map_header_info,
                  const std::map<std::string, std::string> &map_client_info_,
                  std::string &uid,
                  lgx::util::vessel &content, size_t &error_times); // uid for deal with offline
    ~work() {};
    void set_send_data_handler(lgx::util::callback2 send_data_handler);
    void set_error_handler(lgx::util::callback2 error_handler);
    void set_send_file_handler(lgx::util::callback1 send_file_handler);
    void run();
    void handle_get();
    void handle_post();
    void handle_put();

private:
    const std::map<std::string, std::string> &map_header_info_;
    const std::map<std::string, std::string> &map_client_info_;
    lgx::util::vessel &content_;
    std::string session_;
    std::string &uid_;
    size_t &error_times_;
    lgx::util::callback1 send_file_handler_;
    lgx::util::callback2 send_data_handler_;
    lgx::util::callback2 error_handler_;
    std::map<std::string, std::string> map_url_info_;
    std::map<std::string, std::string> map_url_value_info_;
    std::string request_;
    std::string platform_;
    void send_data(const std::string &suffix, const std::string &content);
    void send_json(lgx::util::json &json_obj);
    std::string json_to_string(json &json_obj);
    void send_file(std::string file_name);
    bool parse_url();
    void handle_not_found();
    void response(ResponseCode error_code);
    bool is_dir(const std::string &path);

    std::string get_date_time();

    void chat_register();
    void chat_login();
    void chat_create_group();
    void chat_delete_group();
    void chat_join_group();
    void chat_get_all_group_info();
    void chat_get_all_user_info();
    void chat_get_group_all_member_info();

    void chat_msg_to_group();
    void chat_msg_to_user();

    /*logic*/
    void client_get_server_datetime();
    void client_get_server_info();
};
