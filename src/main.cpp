#include <iostream>
#include <memory>

#include "network.hpp"
#include "network_user.hpp"

using namespace std;

namespace {
  void print_users(ostream& out, const vector<UserLogin>& users) {
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
  auto bus = make_shared<NetworkBus>();
  NetworkServer server(bus);
  // In real life INetwrokBus should implement communication between Network and NetworkServer
  bus->set_server(&server);
  Network network(bus);

  UserLogin luke_login = "Luke", darth_vader_login = "Darth Vader", leia_login = "Leia";
  NetworkUser luke(luke_login, network), darth_vader(darth_vader_login, network), leia(leia_login, network);

  network.register_user(luke.login());
  network.register_user(darth_vader.login());
  network.connect_user(&darth_vader);
  network.connect_user(&luke);

  print_network_stat(cout, network);

  luke.subscribe(darth_vader_login);
  darth_vader.send_message(luke_login, "Luke! I'm your father!");

  darth_vader.subscribe(luke_login);
  luke.send_message(darth_vader_login, "NOOOO!");

  for (const auto& user : {darth_vader, luke, leia}) {
    print_user_feed(cout, user);
    cout << "\n";
  }

  network.register_user(leia_login);
  network.connect_user(&leia);

  print_network_stat(cout, network);

  leia.subscribe(luke_login);
  luke.send_message(leia_login, "I'm gonna defeat my father!");

  for (const auto& user : {darth_vader, luke, leia}) {
    print_user_feed(cout, user);
    cout << "\n";
  }

  return 0;
}