#include <iostream>

struct foo {
  foo() {}
  foo(const foo&) { std::cout << "foo copy constructor called" << std::endl; }
};

int main() {
  foo obj = foo();
}

/*
g++ rvo.cpp -o rvo.o                         // prints nothing.
g++ -fno-elide-constructors rvo.cpp -o rvo.o // prints "foo copy constructor called".
*/
