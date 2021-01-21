#pragma once
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>

#include "common.hpp"

class NetworkServerUserStorage {
 public:
  struct Info {
    std::unordered_set<UserLogin> followers;
    bool connected = false;
  };

  inline bool is_registered(const UserLogin& login) const {
    std::shared_lock lock(mutex_);
    return store_.find(login) != store_.end();
  }

  inline bool is_connected(const UserLogin& login) const {
    std::shared_lock lock(mutex_);
    if (auto it = store_.find(login); it != store_.end()) {
      return it->second.connected;
    }
    return false;
  }

  inline void add(UserLogin login) {
    std::unique_lock lock(mutex_);
    store_[std::move(login)] = Info{};
  }

  inline void connect(const UserLogin& login) {
    std::unique_lock lock(mutex_);
    store_.at(login).connected = true;
  }

  inline void subscribe(UserLogin follower, const UserLogin& target) {
    std::unique_lock lock(mutex_);
    store_.at(target).followers.insert(std::move(follower));
  }

  inline bool has_follower(const UserLogin& target, const UserLogin& follower) {
    std::shared_lock lock(mutex_);
    return store_.at(target).followers.count(follower) > 0;
  }

  inline UserList registered_users() const {
    std::shared_lock lock(mutex_);
    UserList users;
    for (const auto& [login, _] : store_) {
      users.push_back(login);
    }
    return users;
  }

  inline UserList connected_users() const {
    std::shared_lock lock(mutex_);
    UserList users;
    for (const auto& [login, info] : store_) {
      if (info.connected) {
        users.push_back(login);
      }
    }
    return users;
  }

 private:
  std::unordered_map<UserLogin, Info> store_;
  mutable std::shared_mutex mutex_;
};

class NetworkServer {
  std::shared_ptr<INetworkBus> bus_;
  NetworkServerUserStorage user_store_;

  void validate_user_connected(const UserLogin& login) {
    if (!user_store_.is_connected(login)) {
      throw NetworkErrors::UserNotConnectedError(login);
    }
  }

  void validate_user_registered(const UserLogin& login) {
    if (!user_store_.is_registered(login)) {
      throw NetworkErrors::UserNotFoundError(login);
    }
  }

  void validate_message(const Message& msg) {
    if (!user_store_.has_follower(msg.from, msg.to)) {
      throw NetworkErrors::SubscriptionNotFoundError(msg.to + " not a follower of " + msg.from);
    }
  }

 public:
  inline explicit NetworkServer(std::shared_ptr<INetworkBus> bus) : bus_(std::move(bus)) {}

  inline void register_user(UserLogin login) {
    if (!user_store_.is_registered(login)) {
      user_store_.add(std::move(login));
    }
  }

  inline void connect_user(UserLogin login) {
    validate_user_registered(login);
    user_store_.connect(login);
  }

  inline void subscribe(UserLogin follower, UserLogin target) {
    validate_user_registered(target);
    validate_user_registered(follower);
    user_store_.subscribe(std::move(follower), target);
  }

  inline void process_message(Message msg) {
    validate_user_connected(msg.from);
    validate_user_connected(msg.to);
    validate_message(msg);
    bus_->receive_message(msg.to, msg);
    bus_->receive_message(msg.from, msg);
  }

  inline UserList registered_users() const { return user_store_.registered_users(); }

  inline UserList connected_users() const { return user_store_.connected_users(); }
};