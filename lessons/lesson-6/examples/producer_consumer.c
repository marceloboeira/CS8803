#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 3

int buffer[BUFFER_SIZE];  	/* shared buffer */
int add = 0; /* place to add next element */
int rem = 0; /* place to remove next element */
int num = 0; /* number elements in buffer */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  	/* mutex lock for buffer */
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

void *producer (void *param);
void *consumer (void *param);

int main(int argc, char *argv[]) {
  pthread_t tid1, tid2;  /* thread identifiers */
  int i;

  /* create the threads; may be any number, in general */
  if(pthread_create(&tid1, NULL, producer, NULL) != 0) {
    fprintf(stderr, "Unable to create producer thread\n");
    exit(1);
  }

  if(pthread_create(&tid2, NULL, consumer, NULL) != 0) {
    fprintf(stderr, "Unable to create consumer thread\n");
    exit(1);
  }

  /* wait for created thread to exit */
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("Parent quiting\n");

  return 0;
}

/* Produce value(s) */
void *producer(void *param) {
  int i;

  for (i = 1; i <= 10; i++) {
    /* Insert into buffer */
    printf("Producer: waiting for lock\n");
    pthread_mutex_lock(&m); //waits until lock is available
    printf("Producer: lock acquired\n");
    if (num > BUFFER_SIZE) {
      exit(1);  /* overflow */
    }

    while (num == BUFFER_SIZE) {  /* block if buffer is full */
      printf("Producer: buffer is full\n");
      pthread_cond_wait (&c_prod, &m);
      printf("Producer: conditional granted\n");
    }

    /* if executing here, buffer not full so add element */
    buffer[add] = i;
    add = (add+1) % BUFFER_SIZE;
    num++;
    printf("Producer: inserted %d\n", i);

    pthread_mutex_unlock(&m);
    printf("Producer: lock released\n");

    pthread_cond_signal(&c_cons);
    printf("Producer: consumer signaled\n");

    fflush(stdout);
  }

  printf("Producer: quiting\n");
  fflush(stdout);

  return 0;
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param) {
  int i;

  while (1) {
    printf("Consumer: waiting for lock\n");
    pthread_mutex_lock(&m);
    printf("Consumer: lock acquired\n");
    if (num < 0) {
      exit(1);
    } /* underflow */

    while (num == 0) {  /* block if buffer empty */
      printf("Consumer: buffer is empty\n");
      pthread_cond_wait(&c_cons, &m);
      printf("Consumer: conditional granted\n");
    }

    /* if executing here, buffer not empty so remove element */
    i = buffer[rem];
    rem = (rem+1) % BUFFER_SIZE;
    num--;
    printf ("Consumer: received value %d\n", i);
    pthread_mutex_unlock(&m);
    printf ("Consumer: lock released\n");
    pthread_cond_signal(&c_prod);
    printf ("Consumer: producer signaled\n");

    fflush(stdout);
  }

  return 0;
}
