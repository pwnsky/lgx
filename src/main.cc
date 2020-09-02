#include <iostream>
#include "startup.hh"

int main(int argv, char **argc) {

    if(argv < 2) {
        std::cout << "-h get more info" << std::endl;
        //return 0;
    }

    lgx::start_up startup;
    std::string arg = argc[1];
    if(arg == "-h" || arg == "--help") {
        std::cout << "Usage: ./lgx [OPTION...] [SECTION] PAGE...\n"
                     "-r, --run    run lgx server\n"
                     "-s, --stop   stop lgx server\n"
                     "-h, --help   help of lgx server\n"
                     ;
    }else if(arg == "-r" || arg == "--run") {
        startup.run(); // 启动服务
    }else if(arg == "-s" || arg == "--stop") {

    }else if(arg == "-p" || arg == "--print") {

    }else {
        std::cout << "-h get more info" << std::endl;
    }
    return 0;
}
