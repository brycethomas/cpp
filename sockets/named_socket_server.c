#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 5
#define BUF_SIZE 100
#define SOCKET_NAME "/tmp/my_socket"

int main(int argc, char *argv[]) {
  // Implementation adapted from The Linux Programming Interface, Listing 57-4.
  // gcc named_socket_server.c -o named_socket_server  
  struct sockaddr_un addr;

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1) {
    printf("socket() call failed.");
    exit(1);
  }

  if (remove(SOCKET_NAME) == -1 && errno != ENOENT) {
    printf("remove() call failed.");
    exit(1);
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  if (bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
    printf("bind() call failed.");
    exit(1);
  }

  if (listen(fd, BACKLOG) == -1) {
    printf("listen() call failed.");
    exit(1);
  }

  ssize_t numRead;
  char buf[BUF_SIZE];
  for (;;) {
    int cfd = accept(fd, NULL, NULL);
    if (cfd == -1) {
      printf("accept() call failed.");
      exit(1);
    }

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
      if (write(STDOUT_FILENO, buf, numRead) != numRead) {
        printf("write() call failed.");
        exit(1);
      }
    }

    if (numRead == -1) {
      printf("read() call failed.");
      exit(1);
    }
  }
}
