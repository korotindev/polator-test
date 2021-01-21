#pragma once
#include "common.hpp"

class User {
  UserLogin login_;
  MessageFeed feed_;

 public:
  inline explicit User(UserLogin login) : login_(std::move(login)) {}

  inline const UserLogin& login() const { return login_; }

  inline const MessageFeed& message_feed() const { return feed_; }

  inline void receive(Message msg) { feed_.push_back(std::move(msg)); };
};