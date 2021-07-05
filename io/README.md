Notes based largely on Chapter 63 of The Linux Programming Interface
("Alternative I/O Models").

## Working Definitions

Here are some crude ones:

* **Non-blocking I/O**: using e.g. `O_NONBLOCK` on a call, returns an error
  code as the way of informing that I/O isn't currently possible.
* **Asynchronous I/O**: callbacks, signals, events when e.g. I/O becomes
  possible.
* **Alternative I/O Models**: a third term introduced by TLPI, probably to avoid
  overloaded usages of Asynchronous I/O which means different things to
  different people.

## APIs

* `select()`
* `poll()`
* `epoll()`
* Signal-Driven I/O

## Notification Models

* **Level-triggered**: considered ready if possible to perform I/O system call
  without blocking.  (`select()`, `poll()`, `epoll()`).
* **Edge-triggered**: notification provided if there is I/O activity (e.g. new
  input) since last monitored. (`epoll()`, signal-driven I/O).
