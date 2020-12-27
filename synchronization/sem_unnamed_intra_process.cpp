#include <errno.h>
#include <random>
#include <semaphore.h>
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

/* gcc sem_unnamed_intra_process.cpp -lpthread -o sem_unnamed_intra_process \
   -lstdc++ */
int main() {
  // Initialize a thread-shared semaphore with value 0.
  sem_t my_sem;
  int res = sem_init(&my_sem /* *sem */,
                     0       /* pshared */,
                     0       /* value */);
  
  std::thread t1{produce, &my_sem}, t2{consume, &my_sem};
  t1.join();
  t2.join();

  sem_destroy(&my_sem);
  printf("Both threads completed successfully.  Terminating.\n");
  return 0;
}
