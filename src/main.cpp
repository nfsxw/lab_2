#include "tests.h"
#include <iostream>
#include <string>

/*
class Parent {
public:
  virtual int question() = 0;
};

class Child : public Parent {
private:
  int question() override { return 42; }
};

void print(Parent &obj) {
  int n = obj.question();
  std::cout << n << std::endl;
}
*/

int main() {
  //Child obj;
  //print(obj);
  std::string command;
  std::cout << "Enter 'test' to run tests, 'exit' to quit.\n";

  while (true) {
    std::cout << "> ";
    std::cin >> command;

    if (command == "test") {
      runAllTests();
    } else if (command == "exit") {
      break;
    } else {
      std::cout << "Unknown command. Use 'test' or 'exit'.\n";
    }
  }
  return 0;
}