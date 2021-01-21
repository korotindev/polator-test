#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "user.hpp"

namespace NetworkErrors {
  class UserNotFoundError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };
  class SenderNotFound : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };
  class RecipientNotFound : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };
  class CantFindSuchFollowerError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };
};  // namespace NetworkErrors

class Network {
 private:
  struct UserInfo {
    User& user;
    std::unordered_set<User::Login> subscribers;
  };
  std::unordered_map<User::Login, UserInfo> connected_;
  std::unordered_set<User::Login> registered_;

 public:
  inline std::vector<User::Login> registered_users() const;
  inline std::vector<User::Login> connected_users() const;

  inline void register_user(User&);
  inline void connect_user(User&);
  inline void create_subscription(const User& folower, const User& target);
  inline void process_message(Message msg);
};

inline std::vector<User::Login> Network::registered_users() const {
  std::vector<User::Login> users(registered_.begin(), registered_.end());
  return users;
}

inline std::vector<User::Login> Network::connected_users() const {
  std::vector<User::Login> users;
  for (auto [login, _] : connected_) {
    users.push_back(login);
  }
  return users;
}

inline void Network::register_user(User& user) { registered_.insert(user.login()); }

inline void Network::connect_user(User& user) {
  if (registered_.count(user.login()) == 0) {
    throw NetworkErrors::UserNotFoundError(user.login());
  }

  connected_.try_emplace(user.login(), UserInfo{user, {}});
}

inline void Network::create_subscription(const User& folower, const User& target) {
  if (auto it = connected_.find(target.login()); it != connected_.end()) {
    it->second.subscribers.insert(folower.login());
  } else {
    throw NetworkErrors::UserNotFoundError(target.login());
  }
}

inline void Network::process_message(Message msg) {
  auto sender_it = connected_.find(msg.from);
  if (sender_it == connected_.end()) {
    throw NetworkErrors::SenderNotFound(msg.from);
  }

  if (sender_it->second.subscribers.count(msg.to) == 0) {
    throw NetworkErrors::CantFindSuchFollowerError(msg.to);
  }

  auto recipient_it = connected_.find(msg.to);
  if (recipient_it == connected_.end()) {
    throw NetworkErrors::RecipientNotFound(msg.to);
  }

  sender_it->second.user.add_message_to_feed(msg);
  recipient_it->second.user.add_message_to_feed(msg);
};