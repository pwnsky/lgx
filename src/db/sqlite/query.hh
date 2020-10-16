#pragma once
#include "../../base.hh"
#include "../../thread/mutex_lock.hh"
#include "sql.hh"
#include <exception>

class lgx::db::sqlite::query {
public:
    query();
    ~query();
    bool next();
    const char* value(int index);
    bool exec(std::string sql);
    bool bexec(std::string sql);
    bool select(const std::string &table_name, const std::string &key_sql, const std::string &condition = "none");
    bool insert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql);
    bool binsert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql);
    bool update(const std::string &table_name, const std::string &key_sql, const std::string &value_sql, const std::string &condition);
    bool remove(const std::string &table_name, const std::string &condition);
    void operator=(query &mysql_res);
    void   set_sqlite(sqlite3 *sqlite);
    sqlite3 *get_sqlite();
    static void escape(std::string &str);

private:
    char **res_;
    int res_row_ = 0;
    int res_column = 0;
    int number_of_fields_;
    sqlite3 *sqlite_;
    int get_fields();
    void clean();
};

class lgx::db::sqlite::exception : public std::exception {
public:
    const char *what() const throw() {
        return "Get value out of range";
    }
};
