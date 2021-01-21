#include <iostream>
#include <memory>

#include "user.hpp"
#include "network.hpp"
#include "network_server.hpp"
#include "network_connection.hpp"

using namespace std;

namespace {
  void print_users(ostream& out, const vector<User::Login>& users) {
    for (const auto& login : users) {
      out << login << "\n";
    }
  }
  void print_registered(ostream& out, const Network& net) {
    out << "registered users: \n";
    print_users(out, net.registered_users());
  }

  void print_connected(ostream& out, const Network& net) {
    out << "connected users: \n";
    print_users(out, net.connected_users());
  }

  void print_user_feed(ostream& out, const User& user) {
    out << user.login() << "'s feed:\n";
    for (const auto& msg : user.message_feed()) {
      out << msg.to_string() << "\n";
    }
  }

  void print_network_stat(ostream& out, const Network& network) {
    print_registered(out, network);
    out << "\n";
    print_connected(out, network);
    out << "\n";
  }
}  // namespace

int main() {
  NetworkServer remote_server;
  auto connection = make_unique<DirectCallNetworkConnection>(remote_server);
  Network network(move(connection));

  User::Login luke_login = "Luke", darth_vader_login = "Darth Vader", leia_login = "Leia";
  User luke(luke_login), darth_vader(darth_vader_login), leia(leia_login);

  network.register_user(luke);
  network.register_user(darth_vader);
  network.connect_user(darth_vader);
  network.connect_user(luke);

  print_network_stat(cout, network);

  network.create_subscription(luke, darth_vader);
  network.process_message(Message{.from = darth_vader_login, .to = luke_login, .text = "Luke! I'm your father!"});

  network.create_subscription(darth_vader, luke);
  network.process_message(Message{.from = luke_login, .to = darth_vader_login, .text = "NOOOO!"});

  for(const auto& user : {darth_vader, luke, leia}) {
    print_user_feed(cout, user);
    cout << "\n";
  }

  network.register_user(leia);
  network.connect_user(leia);

  print_network_stat(cout, network);

  network.create_subscription(leia, luke);
  network.process_message(Message{.from = luke_login, .to = leia_login, .text = "I'm gonna defeat my father!"});

  for(const auto& user : {darth_vader, luke, leia}) {
    print_user_feed(cout, user);
    cout << "\n";
  }

  return 0;
}