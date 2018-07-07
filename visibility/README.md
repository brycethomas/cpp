File `main.cpp` calls function `fn_one()` which is declared in `one.h`, and
defined in `one.cpp`.  Below are several examples of how these two files can be
compiled and linked.  These are designed to demonstrate how hiding symbols in a
library can lead to link-time failures.

Regular shared library:

```bash
g++ -I. -fpic -shared -o libone.so one.cpp
g++ -I. -L. main.cpp -o main -l one
LD_LIBRARY_PATH=. ./main # prints "one"

objdump -t libone.so | grep fn_one # prints:
# 000000000000088a g     F .text	0000000000000032              _Z6fn_onev
```

Shared library with hidden symbols:

```bash
g++ -fvisibility=hidden -I. -fpic -shared -o libone.so one.cpp
g++ -I. -L. main.cpp -o main -l one # prints:
# /tmp/ccWvSSjS.o: In function `main':
# main.cpp:(.text+0x5): undefined reference to `fn_one()'
# collect2: error: ld returned 1 exit status

objdump -t libone.so | grep fn_one # prints:
# 000000000000085a l     F .text	0000000000000032              _Z6fn_onev
```

Note: the difference in the two obj dumps above is in the 'g' vs. 'l' flags
(global vs. local).
