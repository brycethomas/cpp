#include <iostream>

// Adapted from https://isocpp.org/wiki/faq/dtors#order-dtors-for-members

class base0 {
public:
  ~base0() {
    std::cout << "base0" << std::endl;
  }
};

class base1 {
public:
  ~base1() {
    std::cout << "base1" << std::endl;
  }
};

class member0 {
public:
  ~member0() {
    std::cout << "member0" << std::endl;
  }
};

class member1 {
public:
  ~member1() {
    std::cout << "member1" << std::endl;
  }
};

class local0 {
public:
  ~local0() {
    std::cout << "local0" << std::endl;
  }
};

class local1 {
public:
  ~local1() {
    std::cout << "local1" << std::endl;
  }
};

class derived : base0, base1 {
public:
  member0 m0_;
  member1 m1_;

  ~derived() {
    local0 l0;
    local1 l1;
  }
};

int main() {
  derived d;
}

/*
  g++ destructor_order.cpp -o destructor_order.o
  ./destructor_order.o // prints following to stdout

  local1
  local0
  member1
  member0
  base1
  base0
*/
