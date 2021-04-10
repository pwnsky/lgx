#include "util.hh"

std::string lgx::util::date_time() {
    char time_str[128] = {0};
    struct timeval tv;
    time_t time;
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm *p_time = localtime(&time);
    strftime(time_str, 128, "%Y-%m-%d %H:%M:%S", p_time);
    return std::string(time_str);
}

std::string lgx::util::cat_file(const std::string &file_name) {
    return "";
}

std::string lgx::util::popen(const std::string &cmd) {
    return "";
}
