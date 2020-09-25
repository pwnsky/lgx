#include "log.hh"

lgx::log::log_loop *lgx::data::log_loop;
int lgx::data::log_fd;
std::string lgx::data::log_path;
std::queue<std::string> lgx::data::logs;

void lgx::log::log_io::open() {
    lgx::data::log_fd = ::open(lgx::data::log_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0755);
    //std::cout << "log file: " << lgx::data::log_path.c_str() << '\n';
    if(-1 == lgx::data::log_fd) {
        perror("open log file");
        abort();
    }
}

void lgx::log::log_io::close() {
    ::close(lgx::data::log_fd);
}

void lgx::log::log_io::write() {

    while(lgx::data::logs.size() > 0) {
        std::string log = lgx::data::logs.front();
        //std::cout << "log[" << log << "]\n";
        lgx::data::logs.pop();
        ::write(lgx::data::log_fd, log.data(), log.size());
    }
}
