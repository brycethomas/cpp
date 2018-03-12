#define SOME_MACRO 1234

int main() {
  int x = SOME_MACRO
  return 0;  
}

/*
g++ -E preprocess_only.cpp // prints the following to stdout

# 1 "preprocess_only.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "preprocess_only.cpp"


int main() {
  int x = 1234
  return 0;
}
*/
