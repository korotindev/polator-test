#include <iostream>

#include "network.hpp"
#include "user.hpp"

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
}  // namespace

int main() {
  User luke("Luke"), darth_vader("Darth Vader");
  Network network;

  network.register_user(luke);
  network.connect_user(darth_vader);

  print_registered(cout, network);
  print_connected(cout, network);

  return 0;
}