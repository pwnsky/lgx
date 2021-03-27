#pragma once

#include "../base.hh"


using logger = lgx::log::logger;


class lgx::work::chat {
public:
    chat() {};
    ~chat() {};

private:

};

namespace lgx {
namespace chat {
enum class request_type {
    error = 0,
    register_,
    login,
    keep_connect,
    get_user_info,
    get_all_group_info,
    get_all_user_info,
    get_server_datatime,
    get_server_info,
    send_msg_to_user,
    send_msg_to_group,
    create_group,
};


class request {
public:
    static lgx::chat::request_type to_enum(std::string request);

private:
    static void init();
    static std::unordered_map<std::string, lgx::chat::request_type> umap_type_;
    static pthread_once_t once_control_;
};
}

}
