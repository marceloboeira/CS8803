#include <stdio.h>
#include <pthread.h>
#define NUMBER_OF_THREADS 4

void *threadFunc(void *pArg) {
  int *p = (int*)pArg;
  int threadNumber = *p;

  printf("Thread Number %d\n", threadNumber);

  return 0;
}

int main(void) {
  int i;
  // Declate pThread vector
  pthread_t tid[NUMBER_OF_THREADS];

  // Create Treads - fork()
  for(i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_create(&tid[i], NULL, threadFunc, &i);
  }

  // Wait for Threads - join()
  for(i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}
