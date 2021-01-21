#pragma once
#include <string>

struct Message {
  std::string from;
  std::string to;
  std::string text;
  inline std::string to_string() const;
};

inline std::string Message::to_string() const {
  return "message from " + from + " to " + to + " : " + text;
}