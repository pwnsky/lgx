#include "firewall.hh"

bool lgx::security::firewall::wall(int fd, const std::string &ip) {
    std::string f_ip;
    bool ret = false;
    for(auto iter = forbid_ips_.begin(); iter != forbid_ips_.end(); ++iter) {
        if(iter->second == ip) {
            shutdown(fd, SHUT_RDWR);
            ret = true;
            break;
        }
    }
    return ret;
}
