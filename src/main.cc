#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "start_up.hh"
#include "util/color.hh"

#define UNUSED(var) do { (void)(var); } while (false)
extern std::string lgx::data::config_path;
std::string lgx::data::os_info;
using color = lgx::util::color;
lgx::start_up startup;

void lgx_exit(int s) {
    UNUSED(s);
    startup.stop();
}

void about() {
    std::cout << "\033[40;33mauthor : i0gan\n\033[0m"
              << "\033[40;33memail  : 418894113@qq.com\n\033[0m"
              << "\033[40;33mweb    : i0gan.cn\n\033[0m";
}
void help() {
    std::cout << "Usage: ./lgx [OPTION...] [SECTION] PAGE...\n"
                "-a   about author\n"
                "-c   load configure file\n"
                "-h   help of lgx server\n"
                "-v   check version of lgx server\n"
                 ;
}

void init() {
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(nullptr);
}

void lgx_logo() {
    std::string show;
    show += color::yellow(" ▄▄       ") + color::red("    ▄▄▄▄  ") + color::blue(" ▄▄▄  ▄▄▄ ") + color::fuchsia("|\n");
    show += color::yellow(" ██       ") + color::red("  ██▀▀▀▀█ ") + color::blue("  ██▄▄██  ") + color::fuchsia("|\n");
    show += color::yellow(" ██       ") + color::red(" ██       ") + color::blue("   ████   ") + color::fuchsia("|\n");
    show += color::yellow(" ██       ") + color::red(" ██  ▄▄▄▄ ") + color::blue("    ██    ") + color::fuchsia("|\n");
    show += color::yellow(" ██       ") + color::red(" ██  ▀▀██ ") + color::blue("   ████   ") + color::fuchsia("|\n");
    show += color::yellow(" ██▄▄▄▄▄▄ ") + color::red("  ██▄▄▄██ ") + color::blue("  ██  ██  ") + color::fuchsia("|\n");
    show += color::yellow(" ▀▀▀▀▀▀▀▀ ") + color::red("    ▀▀▀▀  ") + color::blue(" ▀▀▀  ▀▀▀ ") + color::fuchsia("|\n");
    show += color::reset( "\n");
    std::cout << show;
}

int main(int argc, char **argv) {
    lgx_logo();
#ifdef DEBUG
    setbuf(stdout, NULL);
#endif
    ::signal(SIGINT, lgx_exit);
    int opt = 0;
    lgx::data::config_path = DEFAULT_CONFIG_FILE;
    while((opt = getopt(argc, argv,"h::v::a::c:"))!=-1) {
        switch (opt) {
        case 'h': {
            help();
            exit(0);
        } break;
        case 'c': {
            lgx::data::config_path = optarg;
        } break;
        case 'a': {
            about();
            exit(0);
        } break;
        case 'v': {
            std::cout << "lgx version: " << LGX_VERSION << '\n';
            exit(0);
        } break;
        default: {
            std::cout << "-h get more info" << std::endl;
            exit(0);
        }
        }
    }
    startup.run(); // 启动服务
    std::cout << "\033[40;33mlgx quited! \n\033[0m";
    return 0;
}