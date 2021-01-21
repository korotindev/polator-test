#include <memory>

#include "network_bus.hpp"
#include "user.hpp"

class Network {
  std::shared_ptr<INetworkBus> bus_;

 public:
  inline explicit Network(std::shared_ptr<INetworkBus> bus) : bus_(std::move(bus)) {}
  inline void register_user(UserLogin login) { bus_->register_user(std::move(login)); }
  inline void connect_user(User* user) { bus_->connect_user(user); }
  inline void subscribe(UserLogin follower, UserLogin target) {
    bus_->subscribe(std::move(follower), std::move(target));
  }
  inline void process_message(Message msg) { bus_->process_message(std::move(msg)); }

  inline UserList registered_users() const { return bus_->registered_users(); }
  inline UserList connected_users() const { return bus_->connected_users(); }
};