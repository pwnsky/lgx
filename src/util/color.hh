#include "base.hh"
class lgx::util::color {
public:
    static std::string red(const std::string &str) {
        std::string ret_str  = "\033[31m";
        ret_str += str;
        return ret_str;
    }
    static std::string green(const std::string &str) {
        std::string ret_str  = "\033[32m";
        ret_str += str;
        return ret_str;
    }
    static std::string yellow(const std::string &str) {
        std::string ret_str  = "\033[33m";
        ret_str += str;
        return ret_str;
    }
    static std::string blue(const std::string &str) {
        std::string ret_str  = "\033[34m";
        ret_str += str;
        return ret_str;
    }
    static std::string fuchsia(const std::string &str) {
        std::string ret_str  = "\033[35m";
        ret_str += str;
        return ret_str;
    }
    static std::string cyan(const std::string &str) {
        std::string ret_str  = "\033[36m";
        ret_str += str;
        return ret_str;
    }
    static std::string white(const std::string &str) {
        std::string ret_str  = "\033[37m";
        ret_str += str;
        return ret_str;
    }
    static std::string reset(const std::string &str) {
        std::string ret_str  = "\033[0m";
        ret_str += str;
        return ret_str;
    }
}; 
