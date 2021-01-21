#pragma once
#include <string>
#include <vector>

#include "message.hpp"

class User {
 public:
  using Login = std::string;
  inline explicit User(std::string login);
  inline const std::string& login() const;
  inline void add_message_to_feed(Message msg);
  inline const std::vector<Message>& message_feed() const;
  
 private:
  Login login_;
  std::vector<Message> message_feed_;
};

inline User::User(std::string login) : login_(std::move(login)) {}

inline const User::Login& User::login() const { return login_; }

inline void User::add_message_to_feed(Message msg) { message_feed_.push_back(std::move(msg)); }

inline const std::vector<Message>& User::message_feed() const { return message_feed_; }