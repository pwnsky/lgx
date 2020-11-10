#include "work.hh"

std::string lgx::data::root_path;
std::string lgx::data::web_page;
std::string lgx::data::web_404_page;

lgx::work::work::work(const std::map<std::string, std::string> &map_header_info,
                      const std::map<std::string, std::string> &map_client_info,
                      lgx::util::vessel &content, size_t &error_times) :
    map_header_info_(map_header_info),
    map_client_info_(map_client_info),
    content_(content),
    error_times_(error_times),
    send_file_handler_(nullptr),
    send_data_handler_(nullptr) {

}

void lgx::work::work::set_fd(int fd) {
    fd_ = fd;
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
    } catch (std::out_of_range e) {
        logger() << log_dbg("no http method" + std::string(e.what()) + '\n');
        error_times_ ++;
        return;
    }

    if(parse_url() == false) {
        logger() << log_dbg("ERROR_PARSING_URL");
        error_times_ ++;
        response(ResponseCode::ERROR_PARSING_URL);
        return;
    }
    try {
        request_ = map_url_value_info_.at("request");
        platform_ = map_url_value_info_.at("platform");
    } catch (std::out_of_range e) {}
    //std::cout << "method: "<< http_method << " url:[" << map_url_info_["url"] << "]\n";
    if(http_method == "get") {
        handle_get();
    }else if(http_method == "put") {

    }else if(http_method == "post") {
        handle_post();
    }
}

void lgx::work::work::handle_get() {
    std::string path;
    bool get_file = true;
    try {
        path = map_url_info_.at("path");
    }  catch (std::out_of_range e) {
        logger() << log_dbg("map_header_info_[url]" + std::string(e.what()) + '\n');
        error_times_ ++;
        return;
    }

    bool dir = is_dir(path);
    do {
        if(dir && request_.empty())
            path = lgx::data::root_path + path + lgx::data::web_page;
        else if(request_ == "get_datatime") {
            client_get_datetime();
            get_file = false;
        }else if(request_ == "get_info") {
            client_get_info();
            get_file = false;
        }
        else {
            if(dir)
                path = lgx::data::root_path + path + lgx::data::web_page;
            else
                path = lgx::data::root_path + path;
        }
    } while(false);
    // Send get file
    if(get_file)
        send_file(path);

}
bool lgx::work::work::is_dir(const std::string &path) {
    if(path.size() < 1)
        return false;
    else if(path[0] == '/' && path.at(path.size() - 1) == '/')
        return true;
    return false;
}

void lgx::work::work::handle_post() {

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
    url = lgx::crypto::url::decode(url);
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
        //std::cout << "get_one: [" << get_one << "] key: [" << key << "] value: [" << map_url_value_info_[key] << "]\n";
    }
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

void lgx::work::work::client_get_datetime() {
    json json_obj = {
        { "server", SERVER_NAME },
        { "request", request_ },
        { "code", ResponseCode::SUCCESS },
        { "datetime" , get_date_time() },
        { "content" , get_date_time() }
    };
    send_json(json_obj);
}

void lgx::work::work::client_get_info() {
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



