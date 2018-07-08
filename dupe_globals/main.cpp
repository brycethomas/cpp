#include "getter_setter.h"
#include "globals.h"
#include <iostream>

int main() {
  // read before any writes
  std::cout << "i reported by main: " << i << std::endl; // prints 1
  int i_get = get_i();
  std::cout << "i reported by getter: " << i_get << std::endl; // prints 1
  
  std::cout << "setting i to 10 directly." << std::endl; 
  i = 10;

  // read after direct write
  std::cout << "i reported by main: " << i << std::endl;  // prints 10
  i_get = get_i();
  std::cout << "i reported by getter: " << i_get << std::endl; // prints 1

  std::cout << "setting i to 15 via setter." << std::endl;
  set_i(15);

  // read after setter write
  std::cout << "i reported by main: " << i << std::endl; // prints 10
  i_get = get_i();
  std::cout << "i reported by getter: " << i_get << std::endl; // prints 15
}
