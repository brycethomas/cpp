#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/my_socket"
#define BUF_SIZE 100

int main(int argc, char *argv[]) {
  // Implementation adapted from The Linux Programming Interface, Listing 57-4.
  // gcc named_socket_client.c -o named_socket_client
  struct sockaddr_un addr;

  int fd = socket(AF_UNIX, SOCK_STREAM, 0); // AF: Address Family

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX; // sun: Socket Unix
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  if (connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
  {
    printf("connect() call failed.");
    exit(1);
  }

  ssize_t numRead;
  char buf[BUF_SIZE];
  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if (write(fd, buf, numRead) != numRead) {
      printf("failed to write some or all data to socket.");
      exit(1);
    }
  }

  if (numRead == -1) {
    printf("read() call failed.");
    exit(1);
  }

  exit(0);
}
