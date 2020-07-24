#include <atomic>
#include <iostream>
#include <thread>

void unsafe_increment() {
  // Don't do this.
  int cnt = 0;
  auto f = [&]{cnt++;};
  std::thread t1{f}, t2{f}, t3{f};
  t1.join();
  t2.join();
  t3.join();
  std::cout << "unsafe inc: " << cnt << std::endl;
}

void safe_increment() {
  std::atomic<int> cnt(0);
  auto f = [&]{cnt.fetch_add(1, std::memory_order::memory_order_seq_cst);};
  std::thread t1{f}, t2{f}, t3{f};
  t1.join();
  t2.join();
  t3.join();
  std::cout << "safe inc: " << cnt.load() << std::endl;
}

int main() {
  std::cout << "***unsafe increments:***" << std::endl;
  for (size_t i=0; i < 10; i++) {
    unsafe_increment();
  }
  std::cout << "-----------------------------------" << std::endl;
  std::cout << "***safe increments:***" << std::endl;
  for (size_t i=0; i < 10; i++) {
    safe_increment();
  }
}
