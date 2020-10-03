#include <iostream>
#include <signal.h>
#include "startup.hh"
#define UNUSED(var) do { (void)(var); } while (false)

lgx::start_up startup;

void lgx_exit(int s) {
    UNUSED(s);
    startup.stop();
    std::cout << "quited! lgx\n";
}

void about() {
    std::cout << "\033[40;31mauthor : i0gan\n\033[0m"
              << "\033[40;31memail  : 418894113@qq.com\n\033[0m"
              << "\033[40;31mweb    : i0gan.cn\n\033[0m";
}

int main(int argv, char **argc) {
    if(argv < 2) {
        std::cout << "-h get more info" << std::endl;
        return 0;
    }

    ::signal(SIGINT, lgx_exit);
    std::string arg = argc[1];
    if(arg == "-h" || arg == "--help") {
        std::cout << "Usage: ./lgx [OPTION...] [SECTION] PAGE...\n"
                     "-r, --run    run lgx server\n"
                     "-s, --stop   stop lgx server\n"
                     "-h, --help   help of lgx server\n"
                     "-v, --version check version of lgx server\n"
                     "-a, --about  about author\n"
                     ;
    }else if(arg == "-r" || arg == "--run") {
        startup.run(); // 启动服务
    }else if(arg == "-s" || arg == "--stop") {

    }else if(arg == "-p" || arg == "--print") {

    }else if(arg == "-a" || arg == "--about") {
        about();
    }else if(arg == "-v" || arg == "--version") {
		std::cout << "lgx version: " << LGX_VERSION << '\n';			
    }else {
        std::cout << "-h get more info" << std::endl;
    }
    return 0;
}
