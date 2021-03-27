 #include "chat.hh"


std::unordered_map<std::string, lgx::chat::request_type> lgx::chat::request::umap_type_;
pthread_once_t lgx::chat::request::once_control_;

void lgx::chat::request::init() {
    request::umap_type_["login"] = lgx::chat::request_type::login;
    request::umap_type_["send_msg_to_user"] = lgx::chat::request_type::send_msg_to_user;
    request::umap_type_["send_msg_to_group"] = lgx::chat::request_type::send_msg_to_group;
    request::umap_type_["get_all_group_info"] = lgx::chat::request_type::get_all_group_info;
    request::umap_type_["get_all_user_info"] = lgx::chat::request_type::get_all_user_info;
    request::umap_type_["create_group"] = lgx::chat::request_type::create_group;

}

lgx::chat::request_type lgx::chat::request::to_enum(std::string request) {
    pthread_once(&request::once_control_, request::init);
    request_type ret_type = request_type::error;
    try {
        ret_type = request::umap_type_.at(request);
    } catch (std::out_of_range e) {
        ret_type = request_type::error;
    }
    return ret_type;
}
