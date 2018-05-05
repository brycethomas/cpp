Loosely based on
[https://stackoverflow.com/q/25041992/129475](https://stackoverflow.com/q/25041992/129475).
Demonsrates that g++ doesn't truly enforce the One Definition Rule during
linking, it simply uses the definition from the first library it finds it
in.

```bash
g++ -I. -fpic -shared -o libone.so one.cpp
g++ -I. -fpic -shared -o libtwo.so two.cpp
g++ -I. -L. main.cpp -o main_one -l one -l two
g++ -I. -L. main.cpp -o main_two -l two -l one

LD_LIBRARY_PATH=. ./main_one # prints "one"
LD_LIBRARY_PATH=. ./main_two # prints "two"
```
