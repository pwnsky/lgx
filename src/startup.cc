#include "startup.hh"

extern std::string Data::root_path;
extern std::string Data::web_page;
extern std::string Data::web_404_page;

Lgx::StartUp::StartUp() {
}

Lgx::StartUp::~StartUp() {
}
bool Lgx::StartUp::Run() {
    //setbuf(stdout, nullptr);
    if(LoadConfig() == false) {
        std::cout << "Load config file failed!\n" << std::endl;
        abort();
    }

    //std::cout << "global_mysql_ptr" << global_mysql_ptr << '\n';
    if(RunLoggerModule() == false) {
        std::cout << "Run logger module failed" << std::endl;
        abort();
    }

    std::cout << "tcp port: " << port_ << "  number of thread: " << number_of_thread_ << '\n';
    if(false == this->RunNetworkModule()) {
        std::cout << "Run network module failed!\n";
        abort();
    }
    return true;
}

// Load config file
bool Lgx::StartUp::LoadConfig() {
    std::string file_json;
    FILE* config_file_ptr = fopen(DEFAULT_CONFIG_FILE, "r");
    while(!feof(config_file_ptr)) {
        char buffer[MAX_BUF_SIZE];
        int len = fread(buffer, 1, MAX_BUF_SIZE, config_file_ptr);
        file_json += std::string(buffer, len);
    }

    fclose(config_file_ptr);
    //std::cout << "json [" << file_json << "]\n";
    Third::Json obj;
    try{
        obj = Third::Json::parse(file_json);
    } catch(Third::Json::parse_error &e) {
        d_cout << e.what() << '\n';
        return false;
    }
    // Parse json
    try {
        port_ = obj["port"];
        number_of_thread_ = obj["number_of_thread"];
        Data::root_path = obj["root_path"];
        Data::web_page = obj["web_page"];
        Data::web_404_page = obj["web_404_page"];
    }  catch (Third::Json::exception &e) {
        d_cout << e.what() << '\n';
        abort();
    }
    return true;
}
bool Lgx::StartUp::RunNetworkModule() {
    Net::Net net(port_, number_of_thread_);
    net.Start();
    return true;
}

bool Lgx::StartUp::RunLoggerModule() {
    return true;
}
