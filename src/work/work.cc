#include "work.hh"

std::string lgx::data::root_path;
std::string lgx::data::web_page;
std::string lgx::data::web_404_page;
// chat
std::unordered_map<std::string, lgx::chat::user> lgx::data::users;
std::unordered_map<std::string, lgx::chat::group> lgx::data::groups;

lgx::work::work::work(const std::map<std::string, std::string> &map_header_info,
                      const std::map<std::string, std::string> &map_client_info,
                      std::string &uid,
                      lgx::util::vessel &content, size_t &error_times) :
    map_header_info_(map_header_info),
    map_client_info_(map_client_info),
    content_(content),
    uid_(uid),
    error_times_(error_times),
    send_file_handler_(nullptr),
    send_data_handler_(nullptr) {

}

void lgx::work::work::set_send_file_handler(lgx::util::callback1 send_file_handler) {
    send_file_handler_ = send_file_handler;
}

void lgx::work::work::set_send_data_handler(lgx::util::callback2 send_data_handler) {
    send_data_handler_ = send_data_handler;
}

void lgx::work::work::run() {

    std::string http_method;
    try {
        http_method = map_header_info_.at("method");
        session_ = map_client_info_.at("session");
    } catch (std::out_of_range e) {
        logger() << log_dbg("no http method" + std::string(e.what()) + '\n');
        error_times_ ++;
        return;
    }
#ifdef DEBUG
    d_cout << "method: "<< http_method << " url:[" << map_url_info_["url"] << "]\n";
#endif
    if(parse_url() == false) {
        logger() << log_dbg("ERROR_PARSING_URL");
        error_times_ ++;
        response(ResponseCode::ERROR_PARSING_URL);
        return;
    }

    try {
        request_ = map_url_value_info_.at("request");
    } catch (std::out_of_range e) {
        request_ = "none";
    }

    if(http_method == "get") {
        handle_get();
    }else if(http_method == "put") {

    }else if(http_method == "post") {
        handle_post();
    }
}

void lgx::work::work::handle_get() {
    std::string path;
    try {
        path = map_url_info_.at("path");
    }  catch (std::out_of_range e) {
        logger() << log_dbg("map_header_info_[url]" + std::string(e.what()) + '\n');
        error_times_ ++;
        return;
    }


    bool dir = is_dir(path);
    do {
        if(request_ == "none") {
            if(dir)
                path = lgx::data::root_path + path + lgx::data::web_page;
            else
                path = lgx::data::root_path + path;
            send_file(path);
            break;
        }

        lgx::chat::request_type request_type = lgx::chat::request::to_enum(request_);
        switch (request_type) {
        case lgx::chat::request_type::get_server_info: {
            client_get_server_info();
        } break;
        case lgx::chat::request_type::get_server_datatime: {
            client_get_server_datetime();
        } break;
        case lgx::chat::request_type::get_all_group_info: {
            chat_get_all_group_info();
        } break;
        case lgx::chat::request_type::get_all_user_info: {
            chat_get_all_user_info();
        }break;
        default: {
            response(ResponseCode::NO_ACCESS);
        } break;
        }
    } while(false);

}
bool lgx::work::work::is_dir(const std::string &path) {
    if(path.size() < 1)
        return false;
    else if(path[0] == '/' && path.at(path.size() - 1) == '/')
        return true;
    return false;
}

void lgx::work::work::handle_post() {
    //std::cout << "url path: " << map_url_info_["path"] << "\n";
    lgx::chat::request_type request_type = lgx::chat::request::to_enum(request_);
    switch (request_type) {
    case lgx::chat::request_type::login: {
        chat_login();
    } break;
    case lgx::chat::request_type::send_msg_to_user: {
        chat_msg_to_user();
    } break;
    case lgx::chat::request_type::create_group: {
        chat_create_group();
    } break;
    default: {
        response(ResponseCode::NO_ACCESS);
    } break;
    }

}

bool lgx::work::work::parse_url() {
    std::string url;
    std::string value_url;
    std::string path;

    try {
        url = map_header_info_.at("url");
    } catch (std::out_of_range e) {
        logger() << log_dbg("map_header_info_[url]" + std::string(e.what()) + '\n');
        return false;
    }
    map_url_info_["orignal_url"] = url;
    url = lgx::util::url::decode(url);
    map_url_info_["url"] = url;

    int first_value_pos = url.find("?");
    if(first_value_pos > 0) {
        value_url = url.substr(first_value_pos + 1);
        path = url.substr(0, first_value_pos);
    }else {
        path = url;
    }
    map_url_info_["path"] = path;

    // check is have value
    if(value_url.empty()) return true;
    value_url += '&';
    // Get value
    while(true) {
        int key_pos = value_url.find("&");
        if(key_pos < 0) {
            break;
        }
        std::string get_one = value_url.substr(0, key_pos);
        value_url = value_url.substr(key_pos + 1);
        if(get_one.empty()) continue;

        int value_pos = get_one.find('=');
        if(value_pos < 0) continue;
        std::string key = get_one.substr(0, value_pos);
        std::string value = get_one.substr(value_pos + 1);
        if(key.empty() || value.empty()) continue;
        map_url_value_info_[key] = value;

    }

    //std::cout << " url args: " << url;
    //    for (auto a : map_url_value_info_) {
    //        std::cout << "key: " << a.first << " value: " << a.second << "\n";
    //    }
    return true;
}

void lgx::work::work::send_file(std::string file_name) {
    if(send_file_handler_)
        send_file_handler_(file_name);
}

void lgx::work::work::send_data(const std::string &suffix, const std::string &content) {
    if(send_data_handler_)
        send_data_handler_(suffix, content);
}

void lgx::work::work::handle_not_found() {
    send_file(lgx::data::web_404_page);
}

void lgx::work::work::response(ResponseCode code) {
    json json_obj = {
        { "server", SERVER_NAME },
        { "request", request_ },
        { "code", code },
        { "datetime" , get_date_time() }
    };
    send_json(json_obj);
}

void lgx::work::work::send_json(json &json_obj) {
    std::ostringstream json_sstream;
    json_sstream << json_obj;
    std::string data = json_sstream.str();
    send_data(".json", data);
}

std::string lgx::work::work::json_to_string(json &json_obj) {
    std::ostringstream json_sstream;
    json_sstream << json_obj;
    return json_sstream.str();
}


std::string lgx::work::work::get_date_time() {
    char time_str[128] = {0};
    struct timeval tv;
    time_t time;
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm *p_time = localtime(&time);
    strftime(time_str, 128, "%Y-%m-%d %H:%M:%S", p_time);
    return std::string(time_str);
}

void lgx::work::work::client_get_server_datetime() {
    json json_obj = {
        { "server", SERVER_NAME },
        { "request", request_ },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , get_date_time() },
        { "content" , get_date_time() }
    };
    send_json(json_obj);
}

void lgx::work::work::client_get_server_info() {
    std::string client_ip;
    std::string client_port;
    try {
        client_ip = map_client_info_.at("client_ip");
        client_port = map_client_info_.at("client_port");
    }  catch (std::out_of_range e) {}
    json json_obj = {
        { "server", SERVER_NAME },
        { "request", request_ },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , get_date_time() },
        { "content" , {
              { "client_ip", client_ip },
              { "client_port", client_port },
              { "server_name", SERVER_NAME },
              { "server_version", LGX_VERSION },
              { "github", "https://github.com/i0gan/lgx" }
          }}
    };
    send_json(json_obj);
}

void lgx::work::work::chat_register() {

}

void lgx::work::work::chat_login() {
    std::cout << "call lgx::work::work::chat_login\n";
    json recv_json_obj;
    try {
        recv_json_obj = json::parse(content_.to_string());
    }  catch (json::parse_error e) {
        response(ResponseCode::ERROR_PARSING_CONTENT);
        return;
    }

    std::string name;
    try {
        name = recv_json_obj["content"]["name"];
    }  catch (json::type_error) {
        response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }
    // update info
    lgx::chat::user user;
    user.client_session = session_;
    user.name = name;
    user.uid = lgx::util::md5(name).to_lower_case_string();
    lgx::data::users[user.uid] = user;

    json json_obj = {
        { "server", SERVER_NAME },
        { "request", request_ },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , get_date_time() },
        { "content" , { {"uid" , user.uid } } }
    };
    uid_ = user.uid; // 设置http class的uid，用作offline处理
    send_json(json_obj);
}

void lgx::work::work::chat_create_group() {
    json recv_json_obj;
    try {
        recv_json_obj = json::parse(content_.to_string());
    } catch (json::parse_error e) {
        response(ResponseCode::ERROR_PARSING_CONTENT);
        return;
    }

    std::string group_name;
    std::string holder_uid;
    try {
        holder_uid = recv_json_obj["content"]["holder_uid"];
        group_name = recv_json_obj["content"]["group_name"];
    }  catch (json::type_error) {
        response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    // update info
    lgx::chat::group group;
    group.name = group_name;
    group.gid = lgx::util::md5(group_name).to_lower_case_string();
    group.holder_uid = holder_uid;
    group.member_uid_list.push_back(holder_uid);
    lgx::data::groups[group.gid] = group;
}

void lgx::work::work::chat_join_group() {
    json recv_json_obj;
    try {
        recv_json_obj = json::parse(content_.to_string());
    } catch (json::parse_error e) {
        response(ResponseCode::ERROR_PARSING_CONTENT);
        return;
    }

    std::string uid;
    std::string gid;
    try {
        uid = recv_json_obj["content"]["uid"];
        gid = recv_json_obj["content"]["gid"];
    }  catch (json::type_error) {
        response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    auto group_iter= lgx::data::groups.find(gid);
    if(group_iter == lgx::data::groups.end()) {
        return;
    }
    group_iter->second.member_uid_list.push_back(uid);
}

void lgx::work::work::chat_get_all_group_info() {
#ifdef DEUBG
    d_cout << "call chat_get_all_group_info\n";
#endif

    util::json sj = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", "recv_group_msg"},
        { "datetime" , lgx::util::date_time() },
        { "platform", platform_ },
        { "content-type", "msg"},
        { "content", {
          }}
    };
    for (auto &i : lgx::data::groups) {
        util::json g_json = {
            {"name", i.second.name },
            {"gid", i.second.gid}
        };
        sj["content"].push_back(g_json);
    }
}

void lgx::work::work::chat_get_all_user_info() {
#ifdef DEUBG
    d_cout << "call chat_get_all_user_info\n";
#endif
    util::json sj = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", request_},
        { "datetime" , lgx::util::date_time() },
        { "platform", platform_ },
        { "content-type", "user_info"},
        { "content", {
          }}
    };
    for (auto &i : lgx::data::users) {
        std::string online = "online";
        if(i.second.client_session == "none") {
            online = "offline";
        }
        util::json g_json = {
            {"name", i.second.name },
            {"uid", i.second.uid },
            {"network",  online}
        };
        sj["content"].push_back(g_json);
    }
    send_json(sj);
}

void lgx::work::work::chat_get_group_all_member_info() {

}

void lgx::work::work::chat_msg_to_group() {
    json recv_json_obj;
    try {
        recv_json_obj = json::parse(content_.to_string());
    } catch (json::parse_error e) {
        response(ResponseCode::ERROR_PARSING_CONTENT);
        return;
    }
    std::string uid;
    std::string gid;
    std::string msg;
    try {
        uid = recv_json_obj["content"]["uid"];
        gid = recv_json_obj["content"]["gid"];
        msg = recv_json_obj["content"]["msg"];
    } catch (json::type_error) {
        response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    // 查找group
    auto group_iter= lgx::data::groups.find(gid);
    if(group_iter == lgx::data::groups.end()) {
        return;
    }

    util::json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", "recv_group_msg"},
        { "datetime" , lgx::util::date_time() },
        { "platform", platform_ },
        { "content-type", "msg"},
        { "content", {
              {"uid", uid},
              {"gid", gid},
              {"msg", msg},
              {"type", "group"},
          }}
    };
    std::string send_content = json_to_string(send_json);

    // 循环发送
    for(auto m_uid : group_iter->second.member_uid_list) {
        // 查找 user
        auto user_iter= lgx::data::users.find(m_uid);
        if(user_iter == lgx::data::users.end()) {
            continue;
        }
        //查找 session
        auto session_iter= lgx::data::sessions.find(user_iter->second.client_session);
        if(session_iter == lgx::data::sessions.end()) {
            continue;
        }

        // 发送消息
        if(!session_iter->second.expired()) {
            lgx::net::sp_http sp = session_iter->second.lock();
            sp->push_data(send_content);
        }
    }
}




void lgx::work::work::chat_msg_to_user() {
#ifdef DEBUG
    d_cout << "call lgx::work::work::chat_msg_to_user\n";
#endif
    json recv_json_obj;
    try {
        recv_json_obj = json::parse(content_.to_string());
    }  catch (json::parse_error e) {
        response(ResponseCode::ERROR_PARSING_CONTENT);
        return;
    }
    std::string uid;
    std::string tid;
    std::string msg;
    try {
        uid = recv_json_obj["content"]["uid"];
        tid = recv_json_obj["content"]["tid"];
        msg = recv_json_obj["content"]["msg"];
    } catch (json::type_error) {
        response(ResponseCode::ERROR_JSON_CONTENT_TYPE);
        return;
    }

    auto user_iter= lgx::data::users.find(tid);
    if(user_iter == lgx::data::users.end()) {
        response(ResponseCode::FAILURE);
        return;
    }

    //查找 session
    auto session_iter= lgx::data::sessions.find(user_iter->second.client_session);
    if(session_iter == lgx::data::sessions.end()) {
        response(ResponseCode::FAILURE);
        return;;
    }

    // get sender name
    user_iter = lgx::data::users.find(uid);
    if(user_iter == lgx::data::users.end()) {
        response(ResponseCode::FAILURE);
        return;
    }
    std::string name = user_iter->second.name;

    util::json send_json = {
        { "server", SERVER_NAME },
        { "code", ResponseCode::SUCCESS },
        { "request", "recv_user_msg"},
        { "datetime" , lgx::util::date_time() },
        { "platform", platform_ },
        { "content-type", "msg"},
        { "content", {
              {"uid", uid},
              {"msg", msg},
              {"name", name},
              {"type", "user"},
          }}
    };
    std::string send_content = json_to_string(send_json);
    // 发送消息
    if(!session_iter->second.expired()) {
        lgx::net::sp_http sp = session_iter->second.lock();
        sp->push_data(send_content);
        d_cout << "send ok\n";
    }
    response(ResponseCode::SUCCESS);
}



