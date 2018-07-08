Here we demonstrate a pathological compilation/linking scenario that results in
duplicate copies of the global integer `i`.  No error is thrown during
compilation, linking, or loading.  Rather, the symptom silently arises during
program execution, where updates to `i` in one part of the code fail to be
reflected in other parts of the code.

To observe this behavior, you should compile, link, and run the program as
follows:

```bash
g++ -I. -fpic -shared -o libgetter_setter.so getter_setter.cpp

g++ -fvisibility=hidden -o main.o -c main.cpp
g++ main.o -o main -L. -l getter_setter
LD_LIBRARY_PATH=. ./main
```

You should see printed:

```
i reported by main: 1
i reported by getter: 1
setting i to 10 directly.
i reported by main: 10
i reported by getter: 1
setting i to 15 via setter.
i reported by main: 10
i reported by getter: 15
```

The problem here is that when the program is executed, the shared library
`libgetter_setter.so` gets its own copy of `i`, separate from the copy in
`main`.  As such, updates to `i` from code in the shared library are not
reflected in main, and vice versa.

Note: the duplicate global problem goes away if the command from above to
compile `main.cpp` is run *without* the `fvisibility=hidden` flag.
