#include <future>
#include <iostream>
#include <numeric>
#include <vector>
#include <thread>

void accum(std::vector<int>::iterator first,
           std::vector<int>::iterator last,
           std::promise<int> accumulate_promise) {
  int sum = std::accumulate(first, last, 0);
  accumulate_promise.set_value(sum);
}

// g++ promise.cpp -pthread -o promise

int main() {
  std::vector<int> vec1 = {1, 2, 3, 4, 5};
  std::vector<int> vec2 = {2, 4, 6, 8, 10};

  std::promise<int> accum_prom1;
  std::promise<int> accum_prom2;

  std::future<int> accum_future1 = accum_prom1.get_future();
  std::future<int> accum_future2 = accum_prom2.get_future();

  std::thread thread1(accum, vec1.begin(), vec1.end(), std::move(accum_prom1));
  std::thread thread2(accum, vec2.begin(), vec2.end(), std::move(accum_prom2));

  std::cout << "sum of two vecs: "
            << accum_future1.get() + accum_future2.get()
            << std::endl;

  // Promises resolved on threads can in theory return before the thread is
  // finished.  We join here as a courtesy for clean thread exits, even though
  // in this program there's no other work in the threads after promise
  // resolution.
  thread1.join();
  thread2.join();
}
