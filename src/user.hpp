#pragma once
#include <string>
#include <vector>

class User {
 public:
  using Login = std::string;
  inline explicit User(std::string login);
  inline const std::string& login() const;

 private:
  Login login_;
};

inline User::User(std::string login) : login_(std::move(login)) {}

inline const User::Login& User::login() const { return login_; }
