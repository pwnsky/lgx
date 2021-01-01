#pragma once
#include <mysql/mysql.h>
#include <iostream>
#include "../../base.hh"

#define MYSQL_DEFAULT_CONNECT_PORT 3306
class lgx::db::mysql::sql {
        public:
        sql() {}
        ~sql() {}
        bool connect(std::string host,
                std::string user,
                std::string password,
                std::string name,
                int port = MYSQL_DEFAULT_CONNECT_PORT,
                const char *unix_socket = nullptr,
                std::size_t client_flag = 0);
        void disconnect();
        void ping();
        MYSQL *get() {
            return &mysql_;
        }
private:
    MYSQL mysql_;
};

