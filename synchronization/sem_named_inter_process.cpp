#include <errno.h>
#include <fcntl.h>
#include <random>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>

void random_sleep() {
  std::mt19937_64 seed{std::random_device{}()};
  std::uniform_int_distribution<> dist{5, 25};
  std::this_thread::sleep_for(std::chrono::milliseconds{dist(seed)});
}

void produce(std::string sem_name) {
  int written = 0;
  sem_t* sem_ptr = sem_open(sem_name.c_str(), 0);
  if (sem_ptr == SEM_FAILED) {
    printf("Failed to open semaphore in producer. Error: %i\n", errno);
    exit(1);
  }
  while (written < 10) {
    random_sleep();
    if (sem_post(sem_ptr) != 0) {
      printf("Failed to post semaphore. Error: %i.\n", errno);
      exit(1);
    }
    printf("sem_post completed.\n");
    written++;
  }
  printf("All sems posted.  Terminating thread.\n");
}

void consume(std::string sem_name)  {
  int read = 0;
  sem_t* sem_ptr = sem_open(sem_name.c_str(), 0);
  if (sem_ptr == SEM_FAILED) {
    printf("Failed to open semaphore in consumer. Error: %i.\n", errno);
  }
  while (read < 10) {
    random_sleep();
    if (sem_wait(sem_ptr) != 0) {
      printf("Failed to wait on semaphore. Error: %i\n", errno);
      exit(1);
    }
    printf("sem_wait completed.\n");
    read++;
  }
  printf("All sems waited on.  Terminating thread.\n");
}

int main() {
  std::string sem_name = "/my_sem";
  sem_t* sem_ptr = sem_open(sem_name.c_str(),
                            O_CREAT | O_EXCL,
                            S_IRUSR | S_IWUSR,
                            0);
  if (sem_ptr  == SEM_FAILED) {
    printf("Initial attempt to open semaphore failed. Error: %i.\n", errno);
  }

  pid_t pid = fork();
  if (pid != 0) { // parent
    produce(sem_name.c_str());
    // Parent is done with the semaphore at this point.
    if(sem_close(sem_ptr) != 0) {
      printf("sem_close failed in producer.\n");
    }

    // Wait for the child to complete.
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) == false) {
      printf("Child exited abnormally.\n");
      exit(1);
    }
  } else { // child
    consume(sem_name.c_str());
    // Child is done with the semaphore at this point.
    if(sem_close(sem_ptr) != 0) {
      printf("sem_close failed in consumer.\n");
    }
    exit(0);
  }
  // Both parent and child are done with the semaphore at this point.
  if(sem_unlink(sem_name.c_str()) != 0) {
    printf("sem_unlink call failed.\n");
  }
  printf("Both parent and child completed successfully.  Terminating.\n");
  return 0;
}
