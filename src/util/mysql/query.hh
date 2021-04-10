#pragma once
#include "base.hh"
#include "thread/mutex_lock.hh"
#include "sql.hh"

#include <exception>

extern lgx::util::mysql::sql *lgx::data::mysql::sql;
extern lgx::thread::mutex_lock lgx::data::mysql::lock;
class lgx::util::mysql::query {
public:
    query();
    ~query();
    bool next();
    const char* value(int index);
    bool exec(std::string sql);
    bool select(const std::string &table_name, const std::string &key_sql, const std::string &condition = "none");
    bool insert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql);
    bool update(const std::string &table_name, const std::string &key_sql, const std::string &value_sql, const std::string &condition);
    bool remove(const std::string &table_name, const std::string &condition);
    void operator=(query &mysql_res);
    void   set_mysql(MYSQL *mysql);
    MYSQL *get_mysql();
    static void escape(std::string &str);

private:
    MYSQL_RES *res_;
    MYSQL_ROW row_;
    int number_of_fields_;
    MYSQL *mysql_;
    int get_fields();
    void clean();
};

class lgx::util::mysql::exception : public std::exception {
public:
    const char *what() const throw() {
        return "Get value out of range";
    }
};
