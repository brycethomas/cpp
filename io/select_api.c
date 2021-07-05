#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  fd_set readfds, writefds;
  int nfds;
  int iterations = 10;
  char template[] = "/tmp/tmpfileXXXXXX";

  nfds = 0;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);

  printf("creating temporary files\n");
  for (int i = 0; i < iterations; i++) {
    char template_cp[sizeof(template)];
    strcpy(template_cp, template);

    int fd = mkstemp(template_cp);
    FD_SET(fd, &readfds);
    FD_SET(fd, &writefds);
    if (fd >= nfds) {
      nfds = fd + 1;
    }
  }

  pid_t pid = fork();
  if (pid == -1) {
    printf("fork() failed\n");
    exit(1);
  }

  if (pid != 0) {
    printf("parent process\n");
    int status;
    int loop_count = 0;
    while (waitpid(-1, &status, WNOHANG) == 0) {
      loop_count++;
    }
    printf("parent loops waiting for child: %i\n", loop_count);
  } else {
    printf("child process\n");
    int ready = select(nfds, &readfds, &writefds, NULL, NULL);
    for (int i = 0; i < nfds; i++) {
      printf("fd %2i read ready? %i write ready ? %i\n", i, FD_ISSET(i,
                                                                    &readfds),
             FD_ISSET(i, &writefds));
    }
  }
}
