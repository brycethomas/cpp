#include <iostream>
#include <string>
#include <utility>

template<typename... Args>
void call_through(void func (Args...), Args... args) {
  func(std::forward<Args>(args)...);
}

void func_one(int foo, std::string bar) {
  std::cout << "foo: " << foo << " bar: " << bar << std::endl;
}

void func_two(int baz, double bik) {
  std::cout << "baz: " << baz << " bik: " << bik << std::endl;
}

// g++ perfect_forwarding.cpp -o perfect_forwarding
int main() {
  call_through(&func_one, 15, std::string("chicken"));
  call_through(&func_two, 17, 11.5);
}
