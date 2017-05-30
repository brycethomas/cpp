#include <iostream>

struct foo {
  foo() {}
  foo(const foo&) { std::cout << "foo copy constructor called" << std::endl; }
};

int main() {
  foo obj = foo();
}

/*
g++ rov.cpp -o rov.o                         // prints nothing.
g++ -fno-elide-constructors rov.cpp -o rov.o // prints "foo copy constructor called".
*/
