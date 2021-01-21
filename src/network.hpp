#pragma once
#include <string>
#include <vector>
#include <memory>

#include "network_connection.hpp"
#include "user.hpp"

class Network {
 public:
  inline explicit Network(std::unique_ptr<INetworkConnection> connection);
  inline std::vector<User::Login> registered_users() const;
  inline std::vector<User::Login> connected_users() const;

  inline void register_user(User&);
  inline void connect_user(User&);
  inline void create_subscription(const User& folower, const User& target);
  inline void process_message(Message msg);

 private:
  std::unique_ptr<INetworkConnection> connection_;
};

inline Network::Network(std::unique_ptr<INetworkConnection> connection) : connection_(std::move(connection)) {}

inline std::vector<User::Login> Network::registered_users() const { return connection_->registered_users(); }

inline std::vector<User::Login> Network::connected_users() const { return connection_->connected_users(); }

inline void Network::register_user(User& user) { connection_->register_user(user); }

inline void Network::connect_user(User& user) { connection_->connect_user(user); }

inline void Network::create_subscription(const User& folower, const User& target) {
  connection_->create_subscription(folower, target);
}

inline void Network::process_message(Message msg) { connection_->process_message(std::move(msg)); };