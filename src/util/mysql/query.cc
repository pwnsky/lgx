#include "query.hh"
lgx::util::mysql::sql *lgx::data::mysql::sql = nullptr;
lgx::thread::mutex_lock lgx::data::mysql::lock;

lgx::util::mysql::query::query() :
    res_(nullptr),
    mysql_(lgx::data::mysql::sql->get()) {
}

void lgx::util::mysql::query::query::operator=(query &q) {
    row_ = q.row_;
    res_ = q.res_;
    q.clean(); // avoid double free
}

lgx::util::mysql::query::~query() {
    if(!res_) {
        mysql_free_result(res_); //release res
    }
}

void lgx::util::mysql::query::clean() {
    row_ = 0;
    res_ = nullptr;
    number_of_fields_ = 0;
    mysql_ = nullptr;
}

bool lgx::util::mysql::query::exec(std::string sql) {
    int ret = true;
    if(mysql_real_query(mysql_, sql.data(), sql.size())) {
        std::cout << "mysql_real_query Exec : " << mysql_error(mysql_) << " [" << sql << "] " << std::endl;
        ret = false;
    }
    return ret;
}

bool lgx::util::mysql::query::select(const std::string &table_name, const std::string &key_sql, const std::string &condition) {
    std::string sql;
    sql = "SELECT " + key_sql;
    sql += " FROM " + table_name;
    if(condition != "none")
        sql += " WHERE " + condition;
    sql += " ;";
    //std::cout << " " << sql << '\n';
    if(mysql_real_query(mysql_, sql.data(), sql.size())) {
        std::cout << "mysql_real_query Select: " << mysql_error(mysql_) << " [" << sql << "] " << std::endl;
        return false;
    }
    if(res_) mysql_free_result(res_);
    if(!(res_ = mysql_store_result(mysql_))) {
        std::cout << "mysql_store_result : " << mysql_error(mysql_) << std::endl;
    }
    number_of_fields_ = static_cast<int>(mysql_num_fields(res_));
    return true;
}

bool lgx::util::mysql::query::insert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::mysql::lock);
    std::string sql;
    sql = "INSERT INTO " + table_name;
    sql += " (" + key_sql + ") ";
    sql += "VALUE(" + value_sql + ");";
    return exec(sql);
}

bool lgx::util::mysql::query::update(const std::string &table_name, const std::string &key_sql, const std::string &value_sql, const std::string &condition) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::mysql::lock);
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

bool lgx::util::mysql::query::remove(const std::string &table_name, const std::string &condition) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::mysql::lock);
    std::string sql;
    sql = "DELETE FROM " + table_name;
    sql += " WHERE " + condition + ';';
    return exec(sql);
}

bool lgx::util::mysql::query::next() {
    lgx::thread::mutex_lock_guard mutex_lock_guard(lgx::data::mysql::lock);
    bool result = false;
    do {
        if(!res_) {
            break;
        }
        row_ = mysql_fetch_row(res_);
        if(row_) {
            result = true;
            break;
        }
    } while (false);
    return result;
}

const char* lgx::util::mysql::query::value(int index) {
    if((number_of_fields_ < index + 1 ) && !row_) {
        std::cout << "out_of_range: " << index << " / " << number_of_fields_ << std::endl;
        throw exception();
        return nullptr;
    }
    if(!row_[index]) {
        return "none";
    }
    return row_[index];
}

void lgx::util::mysql::query::set_mysql(MYSQL *mysql) {
    mysql_ = mysql;
}
MYSQL *lgx::util::mysql::query::get_mysql() {
    return mysql_;
}

void lgx::util::mysql::query::escape(std::string &str) {
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
