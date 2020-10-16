#include "query.hh"

lgx::db::sqlite::sql *lgx::data::sqlite::sql = nullptr;
lgx::thread::mutex_lock lgx::data::sqlite::lock;

lgx::db::sqlite::query::query() :
    //res_(nullptr),
    sqlite_(lgx::data::sqlite::sql->get()) {
}

void lgx::db::sqlite::query::query::operator=(query &q) {
//    row_ = q.row_;
//    res_ = q.res_;
//    q.clean(); // avoid double free
}

lgx::db::sqlite::query::~query() {
//    if(!res_) {
//        mysql_free_result(res_); //release res
//    }
}

void lgx::db::sqlite::query::clean() {
//    row_ = 0;
//    res_ = nullptr;
//    number_of_fields_ = 0;
//    mysql_ = nullptr;
}

bool lgx::db::sqlite::query::exec(std::string sql) {
    int ret = true;
    char *err_msg;
    ret = sqlite3_exec(sqlite_, sql.data(), nullptr, nullptr, &err_msg) != SQLITE_OK;
    if(ret) {
        std::cout << "sqlite_real_query exec : " << err_msg << " [" << sql << "] " << std::endl;
        ret = false;
    }
    sqlite3_free(err_msg);
    return ret;
}


bool lgx::db::sqlite::query::select(const std::string &table_name, const std::string &key_sql, const std::string &condition) {
    bool ret = true;
    std::string sql;
    sql = "SELECT " + key_sql;
    sql += " FROM " + table_name;
    if(condition != "none")
        sql += " WHERE " + condition;
    sql += " ;";
    char *err_msg;
    //std::cout << " " << sql << '\n';
    if(res_) sqlite3_free_table(res_);
    bool ret_result = sqlite3_get_table(sqlite_, sql.c_str(), &res_, &res_row_, &res_column, &err_msg);
    if(ret_result != SQLITE_OK) {
        std::cout << "sqlite get_table: " << err_msg << " [" << sql << "] " << std::endl;
        ret = false;
    }
    sqlite3_free(err_msg);
    return ret;
}

bool lgx::db::sqlite::query::insert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::sqlite::lock);
    std::string sql;
    sql = "INSERT INTO " + table_name;
    sql += " (" + key_sql + ") ";
    sql += "VALUE(" + value_sql + ");";
    return exec(sql);
}

bool lgx::db::sqlite::query::binsert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql) {
//    int ret = true;
//    char *err_msg;
//    sqlite3_stmt* stmt;
//    std::string sql = "INSERT INTO " + table_name + " VALUES ";
//    sql += " (" + key_sql + ") ";
//    sql += "VALUE(" + value_sql + ");";

//    sqlite3_prepare(sqlite_,sql.c_str(), -1, &stmt, nullptr);
//    sqlite3_bind_blob(stmt,1,value_sql.data(),value_sql.size(), nullptr);
//    sqlite3_step(stmt);
    return false;
}

bool lgx::db::sqlite::query::update(const std::string &table_name, const std::string &key_sql, const std::string &value_sql, const std::string &condition) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::sqlite::lock);
    std::string sql;
    std::string keys = key_sql + ',';
    std::string values = value_sql + ',';
    sql = "UPDATE " + table_name + " SET ";
    while(true) {
        int key_pos = keys.find(',');
        int value_pos = values.find(',');
        if(key_pos < 0 || value_pos < 0) {
            sql.pop_back();
            break;
        }
        std::string key = keys.substr(0, key_pos);
        std::string value = values.substr(0, value_pos);
        sql += key + "='" + value + "',";
        keys = keys.substr(key_pos + 1);
        values = values.substr(value_pos + 1);
    }
    sql += " WHERE " + condition + ';';
    return exec(sql);
}

bool lgx::db::sqlite::query::remove(const std::string &table_name, const std::string &condition) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::sqlite::lock);
    std::string sql;
    sql = "DELETE FROM " + table_name;
    sql += " WHERE " + condition + ';';
    return exec(sql);
}

bool lgx::db::sqlite::query::next() {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::sqlite::lock);
    bool result = false;
//    do {
//        if(!res_) {
//            break;
//        }
//        row_ = mysql_fetch_row(res_);
//        if(row_) {
//            result = true;
//            break;
//        }
//    } while (false);
    return result;
}

const char* lgx::db::sqlite::query::value(int index) {
//    if((number_of_fields_ < index + 1 ) && !row_) {
//        std::cout << "out_of_range: " << index << " / " << number_of_fields_ << std::endl;
//        throw exception();
//        return nullptr;
//    }
//    if(!row_[index]) {
//        return "none";
//    }
//    return row_[index];
    return nullptr;
}

void lgx::db::sqlite::query::set_sqlite(sqlite3 *sqlite) {
    sqlite_ = sqlite;
}
sqlite3 *lgx::db::sqlite::query::get_sqlite() {
    return sqlite_;
}

void lgx::db::sqlite::query::escape(std::string &str) {
   std::string str_ret;
   for(auto c : str) {
       if(c == '\'')
           str_ret += "\\'";
       else if(c == '"')
           str_ret += "\\\"";
       else if(c == '\b')
           str_ret += "\\b";
       else if(c == '\n')
           str_ret += "\\n";
       else if(c == '\t')
           str_ret += "\\t";
       else
           str_ret += c;
   }
   str = str_ret;
}
