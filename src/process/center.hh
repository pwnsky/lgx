#pragma once

#include "../base.hh"
#include <map>
#include <string>
#include "../json/json.hpp"
#include <unistd.h>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unordered_map>
#include <pthread.h>

using Json = nlohmann::json;

enum class Process::ResponseCode {
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

class Process::Center {
public:
    explicit Center(const std::map<std::string, std::string> &map_header_info, std::string &content); // uid for deal with offline
    ~Center();
    void Process();
    void set_send_data_handler(Util::CallBack2 send_data_handler);
    void set_error_handler(Util::CallBack2 error_handler);
    void set_send_file_handler(Util::CallBack1 send_file_handler);
    void set_fd(int fd) ;

    void HandleGet();
    void HandlePost();
    void HandlePut();
private:
    const std::map<std::string, std::string> &map_header_info_;
    std::string &content_;
    Util::CallBack1 send_file_handler_;
    Util::CallBack2 send_data_handler_;
    Util::CallBack2 error_handler_;
    std::map<std::string, std::string> map_url_info_;
    std::map<std::string, std::string> map_url_value_info_;
    std::string request_;
    std::string platform_;
    int fd_;
    void SendData(const std::string &suffix, const std::string &content);
    void SendJson(Json &json_obj);
    void SendFile(std::string file_name);
    bool ParseUrl();
    void HandleNotFound();
    void Response(Process::ResponseCode error_code);
    /* */

    std::string GetDateTime();
};
