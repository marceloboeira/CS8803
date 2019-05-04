#include <stdio.h>
#include <pthread.h>
#define NUMBER_OF_THREADS 4

void *threadFunc(void *pArg) {
  int threadNumber = *((int*)pArg);
  printf("Thread Number %d\n", threadNumber);

  return 0;
}

int main(void) {
  int i;
  int tNum[NUMBER_OF_THREADS];
  pthread_t tid[NUMBER_OF_THREADS];

  for(i = 0; i < NUMBER_OF_THREADS; i++) {
    tNum[i] = i;
    pthread_create(&tid[i], NULL, threadFunc, &tNum[i]);
  }

  for(i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}
