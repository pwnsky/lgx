#include "eventloop.hh"
__thread lgx::net::eventloop *global_eventloop = nullptr;

lgx::net::eventloop::eventloop() :
    looping_(false),
    awake_fd_(eventloop::create_event_fd()),
    quit_(false),
    event_handling_(false),
    thread_id_(lgx::thread::current_thread::get_tid()),
    sp_epoll_(new epoll()),
    sp_awake_channel_(new channel(this, awake_fd_))
{
    if(global_eventloop) {

    }else {
        global_eventloop = this;
    }
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
    sp_awake_channel_->set_read_handler(std::bind(&eventloop::handle_read, this));
    sp_awake_channel_->set_connected_handler(std::bind(&eventloop::handle_connect, this));
    sp_epoll_->add(sp_awake_channel_, 0);
}

void lgx::net::eventloop::handle_read() {

    // 读取数据传输到缓冲区前的数据
    char buf[8];
    ssize_t read_len = lgx::net::util::read(awake_fd_, &buf, sizeof(buf));
    if(read_len != sizeof (buf)) {
        std::cout << "eventloop::hand_read() reads " << read_len << "instead of 8\n";
    }
    //std::cout << "data: [" << buf << "]\n";
    sp_awake_channel_->set_event(EPOLLIN | EPOLLET);
}

void lgx::net::eventloop::handle_connect() {

}

void lgx::net::eventloop::update_epoll(sp_channel spc, int ms_timeout) {
    sp_epoll_->mod(spc, ms_timeout);
}

void lgx::net::eventloop::add_to_epoll(sp_channel spc, int ms_timeout) {
    sp_epoll_->add(spc, ms_timeout);
}


int lgx::net::eventloop::create_event_fd() {
    int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(event_fd < 0) {
        perror("eventfd:");
        abort();
    }
    return event_fd;
}

void lgx::net::eventloop::loop() {
    assert(looping_ == false);
    assert(is_in_loop_thread());
    looping_ = true;
    quit_ = false;
    std::vector<sp_channel> v_sp_event_channels;
    while(!quit_) {
        v_sp_event_channels.clear();
        v_sp_event_channels = sp_epoll_->get_all_event_channels(); // 获取所有未处理事件
        event_handling_ = true;
        for (auto &sp_channel : v_sp_event_channels) {
            sp_channel->handle_event(); // 处理事件
        }
        event_handling_  = false;
        run_pending_callback_func(); // 运行等待的回调函数
        sp_epoll_->handle_expired_event();
    }
}

void lgx::net::eventloop::quit() {
    quit_ = true;
    if(is_in_loop_thread() == false) {
        wake_up();
    }
}
void lgx::net::eventloop::run_in_loop(lgx::util::callback &&func) {
    if(is_in_loop_thread())
        func();
    else
        push_back(std::move(func));
}

void lgx::net::eventloop::push_back(lgx::util::callback &&func) {
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_);
    pending_callback_functions_.emplace_back(std::move(func));
    if(!is_in_loop_thread() || calling_pending_callback_function_)
        wake_up();
}

//判断是否在事件循环的线程中
bool lgx::net::eventloop::is_in_loop_thread() {
    // 只需通过tid来进行判断, 若是事件循环的线程, 那么是通过Thread创建出来的, 所以Thread::CurrentThread::get_tid()的值是子线程的值,
    // 这就可以判断是否为子线程了, 而thread_id_始终是父线程的id
    return thread_id_ == lgx::thread::current_thread::get_tid();
}

// 从epoll中移除事件
void lgx::net::eventloop::remove_from_epoll(sp_channel sp_channel) {
    sp_epoll_->del(sp_channel);
}

// 用于保持长连接避免断开连接
void lgx::net::eventloop::wake_up() {
    char buf[8];
    ssize_t write_len = lgx::net::util::write(awake_fd_, buf, sizeof (buf));
    if(write_len != sizeof (buf)) {
        d_cout << "eventloop::wakeup write:" << write_len << " instead of 8\n";
    }
}

// 运行待运行的回调函数
void lgx::net::eventloop::run_pending_callback_func() {
    std::vector<lgx::util::callback> v_callback_functions;
    calling_pending_callback_function_ = true;
    lgx::thread::mutex_lock_guard mutex_lock_guard(mutex_lock_); // 保证线程单个线程执行

    //Calling all functions in pending vecotr
    v_callback_functions.swap(pending_callback_functions_); // 获取所有等待的毁掉函数
    // 依次运行回调函数
    for( size_t idx = 0; idx < v_callback_functions.size(); ++idx) {
        v_callback_functions[idx](); // 依次运行
    }
    calling_pending_callback_function_ = false;
}

void lgx::net::eventloop::assert_in_loop_thread() {
    assert(is_in_loop_thread());
}

void lgx::net::eventloop::shutdown(sp_channel spc) {
    lgx::net::util::shutdown_write_fd(spc->get_fd());
}
