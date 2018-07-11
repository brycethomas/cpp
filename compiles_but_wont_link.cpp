#include <iostream>

int get_variable();

int main() {
  int my_var = get_variable();
  std::cout << "The value of my_var is: " << my_var << std::endl;
}

/*
  $ g++ compiles_but_wont_link.cpp -o compiles_but_wont_link.o -c # works.
  $ g++ compiles_but_wont_link.cpp -o compiles_but_wont_link      # link error.

  # /tmp/ccZn61LY.o: In function `main':
  # compiles_but_wont_link.cpp:(.text+0x9): undefined reference to `get_variable()'
  # collect2: error: ld returned 1 exit status
 */
