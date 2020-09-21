#include "startup.hh"

extern std::string lgx::data::root_path;
extern std::string lgx::data::web_page;
extern std::string lgx::data::web_404_page;
extern std::map<std::string, std::string> lgx::data::firewall;
lgx::start_up::start_up() {

}

lgx::start_up::~start_up() {

}
void lgx::start_up::show_logo() {
    std::cout << "\033[40;30m _     ______  __\n\033[0m"
                 "\033[40;31m| |   / ___\\ \\/ /\n\033[0m"
                 "\033[40;32m| |  | |  _ \\  / \n\033[0m"
                 "\033[40;33m| |__| |_| |/  \\ \n\033[0m"
                 "\033[40;34m|_____\\____/_/\\_\\\n\033[0m";
}
bool lgx::start_up::run() {
    //setbuf(stdout, nullptr);
    show_logo();
    if(load_config() == false) {
        std::cout << "Load config file failed!\n" << std::endl;
        abort();
    }

    if(run_logger_module() == false) {
        std::cout << "Run logger module failed" << std::endl;
        abort();
    }

    std::cout << "tcp port: " << port_ << "  number of thread: " << number_of_thread_ << '\n';
    if(false == this->run_network_module()) {
        std::cout << "Run network module failed!\n";
        abort();
    }
    return true;
}

// Load config file
bool lgx::start_up::load_config() {
    std::string file_json;
    FILE* config_file_ptr = fopen(DEFAULT_CONFIG_FILE, "r");
    while(!feof(config_file_ptr)) {
        char buffer[MAX_BUF_SIZE];
        int len = fread(buffer, 1, MAX_BUF_SIZE, config_file_ptr);
        file_json += std::string(buffer, len);
    }

    fclose(config_file_ptr);
    //std::cout << "json [" << file_json << "]\n";
    lgx::third::json obj;
    try{
        obj = lgx::third::json::parse(file_json);
    } catch(third::json::parse_error &e) {
        d_cout << e.what() << '\n';
        return false;
    }
    // Parse json
    try {
        port_ = obj["port"];
        number_of_thread_ = obj["number_of_thread"];
        lgx::data::root_path = obj["root_path"];
        lgx::data::web_page = obj["web_page"];
        lgx::data::web_404_page = obj["web_404_page"];
    }  catch (lgx::third::json::exception &e) {
        d_cout << e.what() << '\n';
        abort();
    }

    // 防火墙禁用特定ip
    auto ips = obj["firewall"];
    for(auto iter = ips.begin(); iter != ips.end(); ++iter) {
        std::string ip_key, ip;
        try {
            ip_key = iter.value();
            ip = iter.value();
        }  catch (third::json::type_error e) {
            d_cout << e.what() << '\n';
            return false;
        }
        lgx::data::firewall[ip_key] = ip;
    }

    return true;
}
bool lgx::start_up::run_network_module() {
    lgx::net::net net(port_, number_of_thread_);
    net.start();
    return true;
}

bool lgx::start_up::run_logger_module() {
    return true;
}
