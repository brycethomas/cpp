#include <iostream>
#include <memory>

struct Foo {
  Foo() {
    std::cout << "Constructing a foo" << std::endl;
  }
};

void deleteFoo(Foo* foo) {
  std::cout << "Foo custom deleter called" << std::endl;
}

int main() {
  std::unique_ptr<Foo, decltype(&deleteFoo)> bla(new Foo(), deleteFoo);
}

/*
g++ custom_deleter.cpp -o custom_deleter.o -std=c++11

// Constructing a foo
// Foo custom deleter called
*/
