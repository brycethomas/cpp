#include <assert.h>
#include <errno.h>
#include <random>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>

void random_sleep() {
  std::mt19937_64 seed{std::random_device{}()};
  std::uniform_int_distribution<> dist{5, 25};
  std::this_thread::sleep_for(std::chrono::milliseconds{dist(seed)});
}

void produce(sem_t* sem) {
  int written = 0;
  while (written < 10) {
    random_sleep();
    int res = sem_post(sem);
    if (res != 0) {
      printf("Failed to post semaphore. Error: %i.\n", errno);
      exit(1);
    }
    printf("sem_post completed.\n");
    written++;
  }
  printf("All sems posted.  Terminating thread.\n");
}

void consume(sem_t* sem) {
  int read = 0;
  while (read < 10) {
    random_sleep();
    int res = sem_wait(sem);
    if (res != 0) {
      printf("Failed to wait on semaphore. Error: %i.\n", errno);
      exit(1);
    }
    printf("sem_wait completed.\n");
    read++;
  }
  printf("All sems waited on.  Terminating thread.\n");
}

int main() {
  // Initialize a process-shared semaphore with value 0.
  sem_t* sem_ptr;
  sem_ptr = (sem_t *)mmap(NULL,
                            sizeof(sem_t),
                            PROT_READ | PROT_WRITE,
                            MAP_ANONYMOUS | MAP_SHARED,
                            0,
                            0);
  assert(sem_ptr != MAP_FAILED);
  
  int res = sem_init(sem_ptr /* *sem */,
                     1       /* pshared */,
                     0       /* value */);

  pid_t pid = fork();
  if (pid != 0) { // parent
    produce(sem_ptr);

    // Wait for the child to complete.
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) == false) {
      printf("Child exited abnormally\n");
      exit(1);
    }    
  } else { // child
    consume(sem_ptr);
    exit(0);
  }

  sem_destroy(sem_ptr);
  assert(munmap(sem_ptr, sizeof(sem_t)) != -1);
  printf("Both parent and child completed successfully.  Terminating.\n");
  return 0;
}
