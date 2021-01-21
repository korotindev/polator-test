#include <unordered_map>

#include "network_server.hpp"
#include "user.hpp"

class NetworkBus : public INetworkBus {
  NetworkServer* server_ = nullptr;
  std::unordered_map<UserLogin, User*> user_closures_;

 public:
  class NullServerError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  inline void set_server(NetworkServer* srv) { server_ = srv; }

  inline void register_user(UserLogin login) override {
    validate_server();
    server_->register_user(login);
  }

  inline void connect_user(User* user) override {
    validate_server();
    server_->connect_user(user->login());
    user_closures_[user->login()] = user;
  }

  inline void subscribe(UserLogin follower, UserLogin target) override {
    validate_server();
    server_->subscribe(std::move(follower), std::move(target));
  }

  inline void process_message(Message msg) override {
    validate_server();
    server_->process_message(std::move(msg));
  }

  inline void receive_message(UserLogin login, Message msg) override {
    if (auto it = user_closures_.find(login); it != user_closures_.end()) {
      it->second->receive(std::move(msg));
    }
  }

  inline UserList registered_users() const {
    validate_server();
    return server_->registered_users();
  }

  inline UserList connected_users() const {
    validate_server();
    return server_->connected_users();
  }

 private:
  inline void validate_server() const {
    if (server_ == nullptr) {
      throw NullServerError("server can't be empty");
    }
  }
};