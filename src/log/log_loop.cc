#include "log_loop.hh"

lgx::log::log_loop::log_loop() : quit_(false) {
    quit_ = false;
    io_.open();
}

lgx::log::log_loop::~log_loop() {
    io_.close();
}
void lgx::log::log_loop::loop() {
    while (!quit_) {
        io_.write();
    }
}

void lgx::log::log_loop::quit() {
    quit_ = true;
}
