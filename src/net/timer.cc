#include "timer.hh"
lgx::net::timer::timer(sp_http sph, int ms_timeout) :
    deleted_(false),
    sp_http_(sph) {

    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    expired_ms_time_ = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

lgx::net::timer::timer(timer &t) :
    expired_ms_time_(0),
    sp_http_(t.sp_http_) {
}


lgx::net::timer::~timer() {
    if(sp_http_) {
        sp_http_->handle_close();
    }
}

time_t lgx::net::timer::get_expired_time() {
    return expired_ms_time_;
}

void lgx::net::timer::update(int ms_timeout) {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    expired_ms_time_ = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000)) + ms_timeout;
}

bool lgx::net::timer::is_valid() {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);
    time_t now_ms_time = (((time_now.tv_sec % 10000) * 1000) + (time_now.tv_usec / 1000));
    if(now_ms_time < expired_ms_time_) {
        return true;
    }else {
        return false;
    }
}

void lgx::net::timer::clear() {
    sp_http_.reset();
    this->set_deleted(); //set as delete will deleted by NetTimerManager::HandleExpiredEvent
}

void lgx::net::timer::set_deleted() {
    deleted_ = true;
}

bool lgx::net::timer::is_deleted() {
    return deleted_;
}


void lgx::net::timer_manager::add_timer(sp_http sph, int ms_timeout) {
    sp_timer sp_net_timer(new timer(sph, ms_timeout));
    sort_sp_timer_queue.push(sp_net_timer);
    sph->bind_timer(sp_net_timer); //set http to link timer
}

void lgx::net::timer_manager::handle_expired_event() {
    while(!sort_sp_timer_queue.empty()) {
        sp_timer sp_net_timer = sort_sp_timer_queue.top();
        if(sp_net_timer->is_deleted()) {
            sort_sp_timer_queue.pop();
        }else if (!sp_net_timer->is_valid()) {
            sort_sp_timer_queue.pop();
        }else
            break;
    }
}

