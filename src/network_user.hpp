#pragma once
#include "network.hpp"
#include "user.hpp"

class NetworkUser : public User {
  Network& network;

 public:
  inline NetworkUser(UserLogin login, Network& network) : User(login), network(network) {}

  inline void send_message(UserLogin to, MessageText text) {
    network.process_message(Message{.from = login(), .to = std::move(to), .text = std::move(text)});
  }

  inline void subscribe(UserLogin target) { network.subscribe(login(), std::move(target)); }
};