#pragma once

#include "../base.hh"
#include <vector>
#include <unistd.h>
#include <iterator>

#include "log.hh"

class lgx::log::log_loop {
public:
    log_loop();
    ~log_loop();
    void loop();
    void quit();
private:
    bool quit_;
    log_io io_;
};
