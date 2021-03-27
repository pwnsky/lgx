#include "http.hh"

std::unordered_map<std::string, std::string> lgx::net::http_content_type::umap_type_;
pthread_once_t lgx::net::http_content_type::once_control_;

const __uint32_t EPOLL_DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 5000;              //ms
const int DEFAULT_KEEP_ALIVE_TIME = 3 * 60 * 1000;  //ms
std::map<std::string, std::weak_ptr<lgx::net::http>> lgx::data::sessions;

void lgx::net::http_content_type::init() {
    //std::cout << "ptrhead_init";
    //init http content type
    http_content_type::umap_type_[".html"] = "text/html";
    http_content_type::umap_type_[".css"] = "text/css";
    http_content_type::umap_type_[".js"] = "application/x-javascript";
    http_content_type::umap_type_[".woff"] = "application/font-woff";
    http_content_type::umap_type_[".woff2"] = "application/font-woff2";
    http_content_type::umap_type_[".avi"] = "video/x-msvideo";
    http_content_type::umap_type_[".bmp"] = "image/bmp";
    http_content_type::umap_type_[".c"] = "text/plain";
    http_content_type::umap_type_[".doc"] = "application/msword";
    http_content_type::umap_type_[".gif"] = "image/gif";
    http_content_type::umap_type_[".gz"] = "application/x-gzip";
    http_content_type::umap_type_[".htm"] = "text/html";
    http_content_type::umap_type_[".ico"] = "image/x-icon";
    http_content_type::umap_type_[".jpg"] = "image/jpeg";
    http_content_type::umap_type_[".png"] = "image/png";
    http_content_type::umap_type_[".txt"] = "text/plain";
    http_content_type::umap_type_[".mp3"] = "audio/mp3";
    http_content_type::umap_type_[".json"] = "application/json";
    http_content_type::umap_type_["default"] = "obj";
}

std::string lgx::net::http_content_type::get_type(const std::string name) {
    ::pthread_once(&http_content_type::once_control_, http_content_type::init);
    if(umap_type_.find(name) == umap_type_.end())
        return umap_type_["default"];
    else
        return umap_type_[name];
}

lgx::net::http::http(int fd,eventloop *elp) :
    fd_(fd),
    eventloop_(elp),
    sp_channel_(new channel(elp, fd)),
    recv_error_(false),
    http_connection_state_(HttpConnectionState::CONNECTED),
    http_process_state_(HttpRecvState::PARSE_HEADER),
    keep_alive_(false) {
    //set callback function handler
    sp_channel_->set_read_handler(std::bind(&http::handle_read, this));
    sp_channel_->set_write_handler(std::bind(&http::handle_write, this));
    sp_channel_->set_reset_handler(std::bind(&http::handle_reset, this));
    session_ = lgx::util::md5(std::to_string(time(nullptr)) + "lgx").to_string();
}
lgx::net::http::~http() {
    close(fd_);
    // delete all
    while(this->out_buffer_queue_.size() > 0) {
            auto buf = out_buffer_queue_.front();
            out_buffer_queue_.pop();
    }

    // fixed bug....
    try {
        auto iter = lgx::data::sessions.find(session_);
        if(iter != lgx::data::sessions.end())
            lgx::data::sessions.erase(iter);
    }  catch (std::out_of_range) {
        std::cout << "~http out of orange!";
    }
    if(uid_ != "none") {
       auto iter = lgx::data::users.find(uid_);
       if(iter!= lgx::data::users.end()) {
           iter->second.client_session = "none";
       }
    }
}

void lgx::net::http::reset() {
    http_process_state_ = HttpRecvState::PARSE_HEADER;
    in_content_buffer_.clear();
    in_buffer_.clear();
    map_header_info_.clear();
    recv_error_ = false;
    if(wp_timer_.lock()) {
        sp_timer sp_net_timer(wp_timer_.lock());
        sp_net_timer->clear();
        wp_timer_.reset();
    }
}

void lgx::net::http::handle_close() {
    http_connection_state_ = HttpConnectionState::DISCONNECTED;
    sp_http guard(shared_from_this()); // avoid delete
    eventloop_->remove_from_epoll(sp_channel_);
}

void lgx::net::http::new_evnet() {
    sp_channel_->set_event(EPOLL_DEFAULT_EVENT);
    eventloop_->add_to_epoll(sp_channel_, DEFAULT_EXPIRED_TIME);
}

void lgx::net::http::bind_timer(sp_timer spt) {
    wp_timer_ = spt;
}

lgx::net::sp_channel lgx::net::http::get_sp_channel() {
    return sp_channel_;
}

lgx::net::eventloop *lgx::net::http::get_eventloop() {
    return eventloop_;
}

void lgx::net::http::unbind_timer() {
    if(wp_timer_.lock()) {
        sp_timer sp_net_timer(wp_timer_.lock());
        sp_net_timer->clear();
        wp_timer_.reset();
    }
}
// 接收数据回调
void lgx::net::http::handle_read() {
    __uint32_t &event = sp_channel_->get_event();
    if(error_times_ > 0 || not_found_times_ > HTTP_MAX_NOT_FOUND_TIMES) {  // check is attacked
        lgx::net::util::shutdown_read_fd(fd_);
        lgx::data::firewall->forbid(client_ip_);
        logger() << "FORBID IP: " + client_ip_ + ":" +  client_port_;
        event = EPOLLET;
        return;
    }

    if(lgx::data::firewall->is_forbid(client_ip_)) {
        lgx::net::util::shutdown_read_fd(fd_);
        event = EPOLLET;
        return;
    }

    // 若消息处于发送状态，则继续发送剩余的数据。
    /*
    if(out_buffer_->size() > 0) {
        handle_write();
        //handle_error((int)HttpResponseCode::NOT_ACCEPTABLE, "Bad recv size");
        return;
    }*/

    do {
        int read_len = lgx::net::util::read(fd_, in_buffer_); // read data
        logger() << "read data from " + client_ip_ + ":" + client_port_;
        //if state as disconnecting will clean th in buffer
        if(http_connection_state_ == HttpConnectionState::DISCONNECTING) {
            //std::cout << "DISCONNECTING\n";
            in_buffer_.clear();
            break;
        }
        if(read_len == 0) { // Disconnected
            http_connection_state_ = HttpConnectionState::DISCONNECTING;
            in_buffer_.clear();
            break;
        }else if(read_len < 0) { // Read data error
            perror("ReadData ");
            recv_error_ = true;
            response_error((int)HttpResponseCode::BAD_REQUEST, "Bad Request");
        }

        // Parse http header
        if(http_process_state_ == HttpRecvState::PARSE_HEADER) {
            HttpParseHeaderResult http_parse_header_result = parse_header();
            // write http header to log
            logger() << "HTTP HEADER:\n[" + header_data_ + "]";
            if(http_parse_header_result == HttpParseHeaderResult::ERROR) {
                logger() << "PARSEHEADER_ERROR";
                recv_error_ = true;
                response_error((int)HttpResponseCode::BAD_REQUEST, "Bad Request");
                error_times_ ++;
                break;
            }
            // Judget if have content data
            //std::cout << "method: " << map_header_info_["method"] << '\n';
            http_process_state_ = HttpRecvState::WORK;
            if(map_header_info_["method"] == "post") {
                content_length_ = 0;
                if(map_header_info_.find("content-length") != map_header_info_.end()) {
                    content_length_ = std::stoi(map_header_info_["content-length"]);
                } else {
                    logger() << log_dbg("not found contnt-length");
                    recv_error_ = true;
                    response_error((int)HttpResponseCode::BAD_REQUEST, "Bad Request");
                    error_times_ ++;
                    break;
                }
                if(content_length_ < 0) {
                    logger() << log_dbg("not found contnt-length");
                    recv_error_ = true;
                    response_error((int)HttpResponseCode::BAD_REQUEST, "Bad Request");
                    error_times_ ++;
                    break;
                }
                //std::cout << "have body data: Cotent-length: " << content_length_ << '\n';
                http_process_state_ = HttpRecvState::RECV_CONTENT;
            }
        }

        // Recv body data
        if(http_process_state_ == HttpRecvState::RECV_CONTENT) {
            // Get content length
            if(!recv_error_ && static_cast<int>(in_buffer_.size()) >= content_length_) {
                http_process_state_ = HttpRecvState::WORK;
            }
        }

        if(http_process_state_ == HttpRecvState::WORK) {
            //std::cout << "left size: " << in_buffer_.size() << '\n';
            handle_work();
            in_buffer_.clear();
            http_process_state_ = HttpRecvState::FINISH;
        }
    } while(false);

    if(recv_error_) {
#ifdef DEBUG
        dbg_log("recv error");
#endif
        this->reset();
    }
    // end
    if(http_process_state_ == HttpRecvState::FINISH) {
        this->reset();
        //if network is disconnected, do not to clean write data buffer, may be it reconnected
    } else if (!recv_error_ && http_connection_state_ == HttpConnectionState::DISCONNECTED) {
        event |= EPOLLIN;
    }
}

lgx::net::HttpParseHeaderResult lgx::net::http::parse_header() {
    lgx::net::HttpParseHeaderResult  result = HttpParseHeaderResult::SUCCESS;
    std::string header_left;
    int first_line_read_pos = 0;
    do {
        int head_end_pos = in_buffer_.find_to_end("\r\n\r\n");
        if(head_end_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        header_data_ = in_buffer_.get_string(0, head_end_pos);
        header_left = header_data_;

        in_buffer_.sub(head_end_pos); // sub header data

        first_line_read_pos = header_left.find("\r\n");
        if(first_line_read_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }
        std::string header_line_1 = header_left.substr(0, first_line_read_pos);
        if(static_cast<int>(header_left.size()) > first_line_read_pos + 2)
            // sub first line str
            header_left = header_left.substr(first_line_read_pos + 2);
        else {
            result = HttpParseHeaderResult::ERROR;
            break;
        }
        // parse http method
        do {
            int http_method_pos = -1;
            if((http_method_pos = header_line_1.find("GET")) >= 0) {
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "get";
                break;
            } else if((http_method_pos = header_line_1.find("POST")) >= 0) {
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "post";
                break;
            } else if((http_method_pos = header_line_1.find("HEAD")) >= 0) {
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "head";
                break;
            } else if((http_method_pos = header_line_1.find("DELETE")) >= 0){
                first_line_read_pos = http_method_pos;
                map_header_info_["method"] = "delete";
                break;
            } else {
                map_header_info_["method"] = "unknown";
                break;
            }
        } while(false);

        if(first_line_read_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        int http_url_start_pos = header_line_1.find('/');
        if(http_url_start_pos < 0) break;
        // sub str
        header_line_1 = header_line_1.substr(http_url_start_pos);

        int http_url_end_pos = header_line_1.find (' ');
        if(http_url_end_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        map_header_info_["url"] = header_line_1.substr(0, http_url_end_pos);

        // Parse http version
        header_line_1 = header_line_1.substr(http_url_end_pos);
        int http_version_pos = header_line_1.find('/');
        if(http_version_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }

        map_header_info_["version"]  = header_line_1.substr(http_version_pos + 1);
    }  while(false);
    // chceck is error
    if (result == HttpParseHeaderResult::ERROR) {
        map_header_info_.clear();
        return result;
    }
    // Parse header key and value
    while(true) {
        int key_end_pos = -1, value_start_pos = -1;
        key_end_pos = header_left.find("\r\n");
        if(key_end_pos < 0) {
            result = HttpParseHeaderResult::ERROR;
            break;
        }
        // sub str
        std::string one_line = header_left.substr(0, key_end_pos);
        header_left = header_left.substr(key_end_pos + 2);

        value_start_pos = one_line.find(':');
        // Last line have not  key and value
        if(value_start_pos < 0) {
            result = HttpParseHeaderResult::SUCCESS;
            break;
        }
        std::string key = one_line.substr(0, value_start_pos);
        std::string value = one_line.substr(value_start_pos + 2);
        // set as lower
        str_lower(key);
        str_lower(value);
        map_header_info_[key] = value;
    }
    return result;
}

void lgx::net::http::handle_work() {
    lgx::work::work w(map_header_info_, map_client_info_, uid_, in_buffer_, error_times_);
    w.set_send_data_handler(std::bind(&http::send_data, this, std::placeholders::_1, std::placeholders::_2));
    w.set_send_file_handler(std::bind(&http::send_file, this, std::placeholders::_1));
    w.run();
}

void lgx::net::http::handle_write() {
    //lgx::thread::mutex_lock_guard guard(mutex_lock_);
    __uint32_t &event = sp_channel_->get_event();

    if(out_buffer_queue_.size() == 0)
        return;

    out_buffer_ = out_buffer_queue_.front();

    /*
    if(event & EPOLLOUT) {
        std::cout << "lgx::net::http::handle_write EPOLLOUT\n";
    } */
    if(http_connection_state_ == HttpConnectionState::DISCONNECTED) {
        return;
    }
#ifdef DEBUG
    dbg_log("write size: " + std::to_string(out_buffer_->size()) + "] end");
#endif
    if(out_buffer_->size() > 0) {
        if(util::write(fd_, out_buffer_) < 0) {
            perror("write header data");
            sp_channel_->set_event(0);
            //out_buffer_->clear();
        }
        if(out_buffer_->size() == 0) { // 数据发送完毕后，若out_buffer_queue_还存在待发送数据，则继续发送
            if(out_buffer_queue_.size() > 0)
                out_buffer_queue_.pop();
            out_buffer_.reset();
            if(out_buffer_queue_.size() > 0) {
                event |= EPOLLOUT; // next round set event as EPOLLOUT
                //std::cout << "剩余。。。。\n";
            }
            return;
        }
    }
    if (out_buffer_->size() > 0)
        event |= EPOLLOUT; // next round set event as EPOLLOUT
}

std::string lgx::net::http::get_suffix(std::string file_name) {
    std::string suffix = file_name;
    while (true) {
        int suffix_pos = suffix.find('.');
        if(suffix_pos < 0)
            break;
        suffix = suffix.substr(suffix_pos + 1);
    }
    suffix = '.' + suffix;
    return suffix;
}

void lgx::net::http::send_data(const std::string &type,const std::string &content) {
    std::shared_ptr<lgx::util::vessel> out_buffer = std::shared_ptr<lgx::util::vessel>(new lgx::util::vessel());
    *out_buffer << "HTTP/1.1 200 OK\r\n";
    if (map_header_info_.find("connection") != map_header_info_.end() &&
            (map_header_info_["connection"] == "keep-alive")) {
        keep_alive_ = true;
        *out_buffer << std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                       std::to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
    }
    *out_buffer << "Cookie: session=" + session_ + "\r\n";
    *out_buffer << "Server: " + std::string(SERVER_NAME) + "\r\n";
    *out_buffer << "Access-Control-Allow-Origin: *\r\n";
    *out_buffer << "Content-Type: " + http_content_type::get_type(type) + "\r\n";
    *out_buffer << "Content-Length: " +  std::to_string(content.size()) + "\r\n";
    *out_buffer << "\r\n";
    *out_buffer << content;
    out_buffer_queue_.push(out_buffer);
    handle_write();
}

// Send file
void lgx::net::http::send_file(const std::string &file_name) {
    if(path_invalid(file_name)) {
        handle_not_found();
        return;
    }
    do {
        if(recv_error_ || http_connection_state_ == HttpConnectionState::DISCONNECTED) {
            break;;
        }
        int fd = open(file_name.c_str(), O_RDONLY);
        if(fd == -1) {
            logger() << log_dbg("Open file [" + file_name + "] failed!");
            handle_not_found();
            not_found_times_ ++;
            break;
        }

        struct stat stat_buf;
        if(fstat(fd, &stat_buf) == -1) {
            response_error((int)HttpResponseCode::SEE_OTHER, "Internal server error");
            close(fd);
            break;;
        }

        // Check this file if as dir, if do not this, when mmap to read data, server will crashed!
        if(S_ISDIR(stat_buf.st_mode)) {
            std::string file_path = file_name.substr(lgx::data::root_path.size());
            redirect(file_path + "/" + lgx::data::web_page); // redirect
            close(fd);
            break;
        }
        // write header
        void* file_mmap_ptr = mmap(nullptr, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

        if(!file_mmap_ptr) {
            close(fd);
            response_error((int)HttpResponseCode::SEE_OTHER, "Internal server error");
            break;
        }
        // Sended it is less than 100 MB
        if(stat_buf.st_size < (1024 * 1024 * 100)) {
            std::shared_ptr<lgx::util::vessel> out_buffer = std::shared_ptr<lgx::util::vessel>(new lgx::util::vessel());
            // get suffix name
            *out_buffer << "HTTP/1.1 200 OK\r\n";
            if (map_header_info_.find("connection") != map_header_info_.end() &&
                    (map_header_info_["connection"] == "keep-alive")) {
                keep_alive_ = true;
                *out_buffer << std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                               std::to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
            }
            *out_buffer << "Cookie: session=" + session_ + "\r\n";
            *out_buffer << "Server: "  + std::string(SERVER_NAME) + "\r\n";
            *out_buffer << "Access-Control-Allow-Origin: *\r\n";
            *out_buffer << "Content-Type: " + http_content_type::get_type(get_suffix(file_name)) + "\r\n";
            *out_buffer << "Content-Length: " +  std::to_string(stat_buf.st_size) + "\r\n";
            *out_buffer << "\r\n";
            out_buffer->append(file_mmap_ptr, stat_buf.st_size);
            out_buffer_queue_.push(out_buffer);
            handle_write();
        }
        munmap(file_mmap_ptr, stat_buf.st_size);
        close(fd);
    } while(false);
}

void lgx::net::http::handle_not_found() {
    std::string file_name = lgx::data::root_path + "/" + lgx::data::web_404_page;
    do {
        if(recv_error_ || http_connection_state_ == HttpConnectionState::DISCONNECTED) {
            break;;
        }
        int fd = open(file_name.c_str(), O_RDONLY);
        if(fd == -1) {
            std::cout << "Open not found file [" << file_name << "] failed!\n";
            response_error((int)HttpResponseCode::NOT_FOUND, "LGX Not Found!");
            break;
        }
        struct stat stat_buf;
        if(fstat(fd, &stat_buf) == -1) {
            response_error((int)HttpResponseCode::SEE_OTHER, "Internal server error");
            close(fd);
            break;
        }
        // Check this file if as dir, if do not this, when mmap to read data, server will crashed!
        if(S_ISDIR(stat_buf.st_mode)) {
            response_error((int)HttpResponseCode::SEE_OTHER, "Error: dir can't get");
            close(fd);
            break;
        }
        void* file_mmap_ptr = mmap(nullptr, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

        if(!file_mmap_ptr) {
            close(fd);
            response_error((int)HttpResponseCode::SEE_OTHER, "Internal server error");
            break;
        }

        std::shared_ptr<lgx::util::vessel> out_buffer = std::shared_ptr<lgx::util::vessel>(new lgx::util::vessel());
        // get suffix name
        *out_buffer << "HTTP/1.1 404 NOT FOUND\r\n";
        if (map_header_info_.find("connection") != map_header_info_.end() &&
                (map_header_info_["connection"] == "keep-alive")) {
            keep_alive_ = true;
            *out_buffer << std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                           std::to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
        }
        *out_buffer << "Cookie: session=" + session_ + "\r\n";
        *out_buffer << "Server: "  + std::string(SERVER_NAME) + "\r\n";
        *out_buffer << "Access-Control-Allow-Origin: *\r\n";
        *out_buffer << "Content-Type: " + http_content_type::get_type(get_suffix(file_name)) + "\r\n";
        *out_buffer << "Content-Length: " +  std::to_string(stat_buf.st_size) + "\r\n";
        *out_buffer << "\r\n";
        out_buffer->append(file_mmap_ptr, stat_buf.st_size);
        out_buffer_queue_.push(out_buffer);
        handle_write();
        munmap(file_mmap_ptr, stat_buf.st_size);
        close(fd);
    } while(false);
}

// 处理连接
void lgx::net::http::handle_reset() {
    int ms_timeout = 0;
    __uint32_t &event = sp_channel_->get_event();
    unbind_timer(); //解除计时器, 避免有两个计时器监视

    if(!recv_error_ && http_connection_state_ == HttpConnectionState::CONNECTED) {
        if(event != 0) {
            if(keep_alive_)
                ms_timeout = DEFAULT_KEEP_ALIVE_TIME;
            else
                ms_timeout = DEFAULT_EXPIRED_TIME;
            if((event & EPOLLIN) && (event & EPOLLOUT)) {
                event = 0;
                event |= EPOLLOUT;
            }
            event |= EPOLLET;
        } else if (keep_alive_) {
            event |= (EPOLLIN | EPOLLET);
            ms_timeout = DEFAULT_KEEP_ALIVE_TIME;
        } else {
            event |= (EPOLLIN | EPOLLET);
            ms_timeout = DEFAULT_KEEP_ALIVE_TIME >> 2;
        }
        eventloop_->update_epoll(sp_channel_, ms_timeout);
    } else if (!recv_error_ && http_connection_state_ == HttpConnectionState::DISCONNECTING
               && (event & EPOLLOUT)) {
        event = (EPOLLOUT | EPOLLET);
    } else {
        eventloop_->run_in_loop(std::bind(&http::handle_close, shared_from_this()));
    }
}

void lgx::net::http::push_data(const std::string &data) {
    // 存在数据正在写入
    std::shared_ptr<lgx::util::vessel> out_buffer = std::shared_ptr<lgx::util::vessel>(new lgx::util::vessel());
    std::string header_buffer;
    header_buffer += "HTTP/1.1 " + std::to_string(static_cast<int>(HttpResponseCode::MOVED_PERMANENTLY)) + "\r\n";
    header_buffer += "Access-Control-Allow-Origin: *\r\n";
    header_buffer += "Server: " + std::string(SERVER_NAME) + "\r\n";
    header_buffer += "Connection: Keep-Alive\r\n";
    header_buffer += "Cookie: session=" + session_ + "\r\n";
    header_buffer += "Content-Length: " + std::to_string(data.size()) + "\r\n";
    header_buffer += "\r\n";
    *out_buffer << header_buffer;
    *out_buffer << data;
    out_buffer_queue_.push(out_buffer);
    wait_event_count_ ++;
#ifdef DEBUG
    dbg_log("push_data: " + data);
#endif
    // 采用目标http的线程进行更新epoll事件
    eventloop_->run_in_loop(std::bind(&http::handle_push_data_reset, shared_from_this()));
}

// 采用单一线程进行更新epoll事件，多线程易出现条件竞争，导致内存错误
void lgx::net::http::handle_push_data_reset() {
    wait_event_count_ --;
    int ms_timeout = 0;
    __uint32_t &event = sp_channel_->get_event();
    unbind_timer(); //解除计时器, 避免有两个计时器监视,重新绑定一个新的计时器
    if(keep_alive_)
        ms_timeout = DEFAULT_KEEP_ALIVE_TIME;
    else
        ms_timeout = DEFAULT_EXPIRED_TIME;
    event |= EPOLLIN | EPOLLET | EPOLLOUT;
    // 回调写入函数
    eventloop_->update_epoll(sp_channel_, ms_timeout); // 更新事件
}

void lgx::net::http::response_error(int error_number, std::string message) {
    std::shared_ptr<lgx::util::vessel> out_buffer = std::shared_ptr<lgx::util::vessel>(new lgx::util::vessel());
#ifdef DEBUG
    d_cout << "start call handle_error\n";
#endif
    message = " " + message;
    std::string header_buffer, body_buffer;
    body_buffer += "<html><title>request error</title>";
    body_buffer += "<body bgcolor=\"ffffff\"><center><hr>";
    body_buffer += "<p>" + std::to_string(error_number) + message + "</p>";
    body_buffer += "<p>" + std::string(SERVER_NAME) + "</p>";
    body_buffer += "<p>" + lgx::util::date_time() + "</p>";
    body_buffer += "<p>lgx src: https://github.com/i0gan/lgx</p>";
    body_buffer += "</hr></center></body></html>";

    header_buffer += "HTTP/1.1 " + std::to_string(error_number) + message + "\r\n";
    //header_buffer += "Access-Control-Allow-Origin: *\r\n";
    header_buffer += "Server: " + std::string(SERVER_NAME) + "\r\n";
    header_buffer += "Connection: Close\r\n";
    header_buffer += "Cookie: session=" + session_ + "\r\n";
    header_buffer += "Content-Type: text/html\r\n";
    header_buffer += "Content-Length: " + std::to_string(body_buffer.size()) + "\r\n";
    header_buffer += "\r\n";
    *out_buffer << header_buffer;
    *out_buffer << body_buffer;
    out_buffer_queue_.push(out_buffer);
    handle_write();
#ifdef DEBUG
    d_cout << "finished call handle_error\n";
#endif
}

void lgx::net::http::str_lower(std::string &str) {
    for (size_t index = 0; index < str.size(); ++index) {
        str[index] = tolower(str[index]);
    }
}

void lgx::net::http::redirect(const std::string &url) {
    std::shared_ptr<lgx::util::vessel> out_buffer = std::shared_ptr<lgx::util::vessel>(new lgx::util::vessel());
    std::string header_buffer;
    header_buffer += "HTTP/1.1 " + std::to_string(static_cast<int>(HttpResponseCode::MOVED_PERMANENTLY)) + "\r\n";
    header_buffer += "Access-Control-Allow-Origin: *\r\n";
    header_buffer += "Server: " + std::string(SERVER_NAME) + "\r\n";
    header_buffer += "Connection: Keep-Alive\r\n";
    header_buffer += "Location: " + url + "\r\n";
    header_buffer += "Content-Length: 0\r\n";
    header_buffer += "\r\n";
    *out_buffer << header_buffer;
    out_buffer_queue_.push(out_buffer);
    handle_write();
}

bool lgx::net::http::path_invalid(const std::string &file_name) {
    char last_ch = 0;
    for (auto ch : file_name) {
        if(ch == '.')
            if(last_ch == '.')
                return true;
    }
    return false;
}
