#pragma once

#include <iostream>
#include "../../base.hh"
#include "../../third/sqlite3.h"

#define MYSQL_DEFAULT_CONNECT_PORT 3306
class lgx::db::sqlite::sql {
        public:
        sql() {}
        ~sql() {}
        bool open(const std::string &db_path);
        void close();
        sqlite3 *get() {
            return sqlite_;
        }
private:
    sqlite3 *sqlite_;
};

