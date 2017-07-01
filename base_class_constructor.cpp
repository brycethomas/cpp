#include <string>
#include <iostream>

class Base {
  public:
    Base(const std::string& name);
  protected:
    std::string _name;
};

Base::Base(const std::string& name) : _name(name) {
  std::cout << "Base class constructor called with argument " << _name << std::endl;
}

class Derived : public Base {
  public:
    Derived();
};

Derived::Derived() : Base("Hardcoded val from derived ctor") {
  std::cout << "Derived class constructor called with base argument " << _name << std::endl;
}

int main() {
  Derived();
}

/*
g++ base_class_constructor.cpp -o base_class_constructor.o

// Base class constructor called with argument Hardcoded val from derived ctor
// Derived class constructor called with base argument Hardcoded val from derived ctor
*/
