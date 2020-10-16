#include "sql.hh"
bool lgx::db::sqlite::sql::open(const std::string &db_path) {
    return sqlite3_open(db_path.c_str(), &sqlite_) != 0;
}

void lgx::db::sqlite::sql::close() {
    sqlite3_close(sqlite_);
}
