#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int do_child(int argc, char **argv);
int do_trace(pid_t child);

// Based heavily on https://blog.nelhage.com/2010/08/write-yourself-an-strace-in-70-lines-of-code/
int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s prog args\n", argv[0]);
    exit(1);
  }

  // create a child process by duplicating the calling process
  pid_t child = fork();

  if (child == 0) { // 0 if this is the child process
    return do_child(argc-1, argv+1);
  } else { // some positive value otherwise
    return do_trace(child);
  }
}

int do_child(int argc, char **argv) {
  char *args [argc+1];
  memcpy(args, argv, argc * sizeof(char*));
  args[argc] = NULL;

  // prepare self to be traced by parent
  ptrace(PTRACE_TRACEME);
  kill(getpid(), SIGSTOP);
  return execvp(args[0], args);
}

int wait_for_syscall(pid_t child);

int do_trace(pid_t child) {
  int status, syscall, retval;
  waitpid(child, &status, 0);
  // only stop for syscalls
  ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
  while(1) {
    if (wait_for_syscall(child) != 0) break;
    // was ORIG_EAX in the guide I followed, but I'm running on a 64-bit
    // machine where the instruction register is RAX.
    syscall = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX);
    fprintf(stderr, "syscall(%d) =", syscall);
    if (wait_for_syscall(child) != 0) break;

    retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RAX);
    fprintf(stderr, "%d\n", retval);
  }
  return 0;
}

int wait_for_syscall(pid_t child) {
  int status;
  while (1) {
    ptrace(PTRACE_SYSCALL, child, 0, 0);
    waitpid(child, &status, 0);
    if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) return 0;
    if (WIFEXITED(status)) return 1;
  }
}

/*
gcc minitrace.c -o minitrace.o

./minitrace.o ./hello_world.o
<prints syscalls made by hello_world executable>
*/

