#include <stdio.h>
#include <pthread.h>
#define NUMBER_OF_THREADS 4

void *hello(void *arg) {
  printf("Hello from the Thread\n");

  return 0;
}

int main(void) {
  int i;
  // Declate pThread vector
  pthread_t tid[NUMBER_OF_THREADS];

  // Create Treads - fork()
  for(i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_create(&tid[i], NULL, hello, NULL);
  }

  // Wait for Threads - join()
  for(i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}
