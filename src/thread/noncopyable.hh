#pragma once
#include "../base.hh"

class lgx::thread::noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable &);
    const noncopyable &operator=(const noncopyable&);
};
