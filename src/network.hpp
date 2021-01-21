#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "user.hpp"

class Network {
 private:
  struct UserInfo {
    std::unordered_set<User::Login> subscriptions;
    bool connected;
  };
  std::unordered_map<User::Login, UserInfo> status_;

 public:
  inline std::vector<User::Login> registered_users() const;
  inline std::vector<User::Login> connected_users() const;

  inline void register_user(const User&);
  inline void connect_user(const User&);
};

inline std::vector<User::Login> Network::registered_users() const {
  std::vector<User::Login> users;
  for (auto [login, _] : status_) {
    users.push_back(login);
  }
  return users;
}

inline std::vector<User::Login> Network::connected_users() const {
  std::vector<User::Login> users;
  for (auto [login, info] : status_) {
    if (info.connected) {
      users.push_back(login);
    }
  }
  return users;
}

inline void Network::register_user(const User& user) { status_.try_emplace(user.login(), UserInfo{{}, false}); }

inline void Network::connect_user(const User& user) {
  if (auto it = status_.find(user.login()); it != status_.end()) {
    it->second.connected = true;
  } else {
    status_.emplace(user.login(), UserInfo{{}, true});
  }
}
