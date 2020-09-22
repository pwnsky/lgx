#include <iostream>
#include "startup.hh"
#define LGX_VERSION "1.2"
int main(int argv, char **argc) {

    if(argv < 2) {
        std::cout << "-h get more info" << std::endl;
        return 0;
    }

    lgx::start_up startup;
    std::string arg = argc[1];
    if(arg == "-h" || arg == "--help") {
        std::cout << "Usage: ./lgx [OPTION...] [SECTION] PAGE...\n"
                     "-r, --run    run lgx server\n"
                     "-s, --stop   stop lgx server\n"
                     "-h, --help   help of lgx server\n"
                     "-v, --version check version of lgx server\n"
                     ;
    }else if(arg == "-r" || arg == "--run") {
        startup.run(); // 启动服务
    }else if(arg == "-s" || arg == "--stop") {

    }else if(arg == "-p" || arg == "--print") {

    }else if(arg == "-v" || arg == "--version") {
		std::cout << "lgx version: " << LGX_VERSION << '\n';			
    }else {
        std::cout << "-h get more info" << std::endl;
    }
    return 0;
}
