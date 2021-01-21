#pragma once
#include <string>
#include <vector>

typedef std::string UserLogin;
typedef std::string MessageText;
typedef std::vector<UserLogin> UserList;

struct Message {
  UserLogin from;
  UserLogin to;
  std::string text;
  inline std::string to_string() const { return "message from " + from + " to " + to + " : " + text; }
};

typedef std::vector<Message> MessageFeed;

namespace NetworkErrors {
  class UserNotFoundError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };
  class UserNotConnectedError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };

  class SubscriptionNotFoundError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };
};  // namespace NetworkErrors

class User;

class INetworkBus {
 public:
  virtual void register_user(UserLogin login) = 0;
  virtual void connect_user(User* user) = 0;
  virtual void subscribe(UserLogin follower, UserLogin target) = 0;
  virtual void process_message(Message msg) = 0;
  virtual void receive_message(UserLogin login, Message msg) = 0;
  virtual UserList registered_users() const = 0;
  virtual UserList connected_users() const = 0;

  virtual ~INetworkBus(){};
};