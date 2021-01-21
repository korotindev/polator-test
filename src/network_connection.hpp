#pragma once
#include <string>
#include <utility>
#include <vector>

#include "message.hpp"
#include "network_server.hpp"
#include "user.hpp"

class INetworkConnection {
 public:
  virtual std::vector<User::Login> registered_users() const = 0;
  virtual std::vector<User::Login> connected_users() const = 0;
  virtual void register_user(User&) = 0;
  virtual void connect_user(User&) = 0;
  virtual void create_subscription(const User& folower, const User& target) = 0;
  virtual void process_message(Message msg) = 0;
};

class DirectCallNetworkConnection : public INetworkConnection {
  NetworkServer& server_;

 public:
  inline explicit DirectCallNetworkConnection(NetworkServer& server) : server_(server) {}

  inline std::vector<User::Login> registered_users() const override { return server_.registered_users(); }

  inline std::vector<User::Login> connected_users() const override { return server_.connected_users(); }

  inline void register_user(User& user) override { server_.register_user(user); }

  inline void connect_user(User& user) override { server_.connect_user(user); }

  inline void create_subscription(const User& folower, const User& target) override {
    server_.create_subscription(folower, target);
  }

  inline void process_message(Message msg) override { server_.process_message(std::move(msg)); }
};