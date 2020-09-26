#include <iostream>
#include <pthread.h>

// Based on https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm
void* print_hi(void* number) {
  std::cout << "hello there number " << (long)number << "." << std::endl;
  return 0;
}

int main() {
  pthread_t my_thread;
  int error_code = pthread_create(&my_thread /*opaque unique id*/,
                                  NULL /*thread attributes*/,
                                  print_hi /*start routine*/,
                                  (void*)12 /*routine argument*/);
  pthread_exit(NULL);
}

/*
gcc pthread.cpp -lpthread -o pthread.o -lstdc++

./pthread.o
<prints "hello there number 12">
*/
