#include <iostream>
#include <memory>

using namespace std;

class SomeClass {
 public:
  SomeClass() {
    x1 = new char[1024];
    x2 = new char[1024 * 1024];
  }

 private:
  char *x1;
  char *x2;
};

int main() {
  SomeClass obj;
  SomeClass other = obj;
  return 0;
}