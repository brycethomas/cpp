#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(int argc, char *argv[]) {
  // gcc socketpair.c -o socketpair
  int fds[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds)) {
    printf("socketpair() call failed.\n");
    exit(1);
  }

  pid_t pid = fork();
  if (pid == -1) {
    printf("fork() call failed.\n");
    exit(1);
  }

  if (pid != 0) {
    printf("parent (server)\n");
    // w/o close, parent will never exit while loop below.
    close(fds[0]);

    ssize_t bytesRxd;
    char buf[BUF_SIZE];
    while ((bytesRxd = read(fds[1], buf, BUF_SIZE)) > 0) {
      if (write(STDOUT_FILENO, buf, bytesRxd) != bytesRxd) {
        printf("write() call failed.\n");
        exit(1);
      }
    }
    if (bytesRxd == -1) {
      printf("read() call failed.\n");
      exit(1);
    }

    close(fds[1]);
    printf("exiting parent\n");
    exit(0);
  } else {
    printf("child (client)\n");
    close(fds[1]);

    ssize_t numRead;
    char buf[BUF_SIZE];
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
      if (write(fds[0], buf, numRead) != numRead) {
        printf("failed to write() some or all data to socket.\n");
        exit(1);
      }
    }

    if (numRead == -1) {
      printf("read() call failed.\n");
      exit(1);
    }

    close(fds[0]); // cleanup
    printf("exiting child\n");
    exit(0);
  }
}
