#pragma once
#include "../base.hh"
#include <sys/socket.h>
class lgx::security::firewall {
public:
    firewall() {}
    ~firewall() {}
    bool wall(int fd, const std::string &ip); // fire wall
    void forbid(std::string key, std::string ip) {
        forbid_ips_[key] = ip;
    };
    void set_forbid_ips(const std::map<std::string, std::string> forbid_ips) {
        forbid_ips_ = forbid_ips;
    }
private:
    std::map<std::string, std::string> forbid_ips_;
};
