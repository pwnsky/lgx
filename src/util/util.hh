#pragma once

#include "base.hh"
#include <sys/time.h>
#include <time.h>
namespace lgx {
namespace util {
std::string date_time();
std::string cat_file(const std::string &file_name);
std::string popen(const std::string &cmd);

}
}
