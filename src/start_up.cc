#include "start_up.hh"

extern std::string lgx::data::root_path;
extern std::string lgx::data::web_page;
extern std::string lgx::data::web_404_page;
extern lgx::log::log *lgx::data::p_log;
extern std::string lgx::data::log_path;
std::string lgx::data::config_path;

std::string lgx::data::protocol;
std::string lgx::data::https_crt_path;
std::string lgx::data::https_key_path;

std::vector<std::string> lgx::data::forbid_ips;
lgx::util::firewall *lgx::data::firewall = nullptr;

lgx::start_up::start_up() :
    sp_log_thread_(new lgx::log::log_thread) {
}

lgx::start_up::~start_up() {
    if(lgx::data::firewall) {
        delete lgx::data::firewall;
    }
}


bool lgx::start_up::stop() {
    net_.stop();
    sp_log_thread_->stop();
    return true;
}

bool lgx::start_up::run() {
    bool error = true;
    //setbuf(stdout, nullptr);
    do {
        if(load_config() == false) {
            std::cout << "Load config file failed!\n" << std::endl;
            logger() << log_dbg("Load config file failed!");
            break;
        }

        if(run_security_module() == false) {
            std::cout << "Run security module failed\n" << std::endl;
            logger() << log_dbg("Run security module failed!");
            break;
        }

        if(run_logger_module() == false) {
            std::cout << "Run logger module failed\n" << std::endl;
            logger() << log_dbg("Run logger module failed!");
            break;
        }

        logger() << "*************  start lgx server...  ***************";
        std::cout << "\nlgx port: " << port_ << "  number of thread: " << number_of_thread_ << '\n';
        if(false == this->run_network_module()) {
            std::cout << "Run network module failed!\n";
            logger() << log_dbg("Run network module failed!");
            break;
        }
        error = false;
    }while(false);
    return error;
}

// Load config file
bool lgx::start_up::load_config() {
    std::string file_json;
    FILE* config_file_ptr = fopen(lgx::data::config_path.c_str(), "r");
    if(config_file_ptr == nullptr) {
        std::cout << "open config file: " << lgx::data::config_path << " failed!\n";
        exit(-1);
    }
    while(!feof(config_file_ptr)) {
        char buffer[MAX_BUF_SIZE];
        int len = fread(buffer, 1, MAX_BUF_SIZE, config_file_ptr);
        file_json += std::string(buffer, len);
    }

    fclose(config_file_ptr);
    //std::cout << "json [" << file_json << "]\n";
    lgx::util::json obj;
    try{
        obj = lgx::util::json::parse(file_json);
    } catch(util::json::parse_error &e) {
        d_cout << e.what() << '\n';
        return false;
    }
    // Parse json
    try {
        port_ = obj["port"];
        number_of_thread_ = obj["number_of_thread"];
        lgx::data::protocol = obj["protocol"];
        lgx::data::root_path = obj["root_path"];
        lgx::data::web_page = obj["web_page"];
        lgx::data::web_404_page = obj["web_404_page"];
        lgx::data::log_path = obj["log"];
    } catch (util::json::exception &e) {
        //d_cout << e.what() << '\n';
        std::cout << "Parse error configure file\n";
        return false;
    }

    if(lgx::data::protocol == "https") {
        lgx::data::https_crt_path = obj["https_crt"];
        lgx::data::https_key_path = obj["https_key"];
    }

    // 防火墙禁用特定ip
    auto ips = obj["firewall"];
    for(auto iter = ips.begin(); iter != ips.end(); ++iter) {
        std::string ip_key, ip;
        try {
            ip_key = iter.value();
            ip = iter.value();
        }  catch (util::json::type_error e) {
            d_cout << e.what() << '\n';
            return false;
        }
        lgx::data::firewall->forbid(ip);
    }
    return true;
}

bool lgx::start_up::run_network_module() {
    if(port_ <= 1024 && getuid() != 0) {
       std::cout << "Must be root" << std::endl;
       return false;
    }
    net_.init(port_, number_of_thread_);
    if(lgx::data::protocol == "https") {
        net_.https_init(lgx::data::https_crt_path, lgx::data::https_key_path);
        net_.start();
    }else if(lgx::data::protocol == "http") {
        net_.start();
    }
    
    return true;
}

bool lgx::start_up::run_logger_module() {
    sp_log_thread_->set_log_path(lgx::data::log_path);
    lgx::data::p_log = sp_log_thread_->creat();
    return true;
}

bool lgx::start_up::run_security_module() {
    lgx::data::firewall = new lgx::util::firewall();
    lgx::data::firewall->set_forbid_ips(lgx::data::forbid_ips);
    return true;
}
