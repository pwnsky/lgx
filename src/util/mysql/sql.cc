#include "sql.hh"
bool lgx::util::mysql::sql::connect(std::string host,
                           std::string user,
                           std::string password,
                           std::string name,
                           int port,
                           const char *unix_socket,
                           std::size_t clientflag) {
    bool return_value = false;
    do {
        if(nullptr == mysql_init(&mysql_)) {
            std::cout << "mysql init fail!"	 << mysql_error(&mysql_) <<std::endl;
            break;
        }
        //connect mysql
        bool is_connect = mysql_real_connect(
                    &mysql_,
                    host.c_str(),
                    user.c_str(),
                    password.c_str(),
                    name.c_str(),
                    port, unix_socket, clientflag);

        if(false == is_connect) {
            std::cout << "mysql_real_connect:" << mysql_error(&mysql_) << std::endl;
            break;
        }
        // 设置为可以重连接mysql
        mysql_options(&mysql_, MYSQL_OPT_RECONNECT, "none");
        return_value = true;
    } while(false);
    return return_value;
}
void lgx::util::mysql::sql::disconnect() {
    mysql_close(&mysql_);
}

void lgx::util::mysql::sql::ping() {
    mysql_ping(&mysql_);
}
