#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_READERS 5
#define NUMBER_OF_WRITERS 5
#define NUMBER_OF_READS 10
#define NUMBER_OF_WRITES 10

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_writer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_reader = PTHREAD_COND_INITIALIZER;

int READERS_COUNTER = 0;
int READERS_WAITING_COUNTER = 0;
unsigned int SHARED_VALUE = 0;

void *reader (void *param);
void *writer (void *param);

void hold() {
  usleep(10000 * (random() % NUMBER_OF_READERS + NUMBER_OF_WRITERS));
  return;
}

int main(int argc, char *argv[]) {
  int reader_num[NUMBER_OF_READERS];
  int writer_num[NUMBER_OF_WRITERS];

  pthread_t readers_tid[NUMBER_OF_READERS];
  pthread_t writers_tid[NUMBER_OF_WRITERS];

  srandom((unsigned int)time(NULL));

  for(int i = 0; i < NUMBER_OF_READERS; i++) {
    reader_num[i] = i;
    pthread_create(&readers_tid[i], NULL, reader, &reader_num[i]);
  }

  for(int i = 0; i < NUMBER_OF_WRITERS; i++) {
    writer_num[i] = i;
    pthread_create(&writers_tid[i], NULL, writer, &writer_num[i]);
  }

  for(int i = 0; i < NUMBER_OF_WRITERS; i++) {
    pthread_join(writers_tid[i], NULL);
  }

  for(int i = 0; i < NUMBER_OF_READERS; i++) {
    pthread_join(readers_tid[i], NULL);
  }

  return 0;
}

void *reader(void *param) {
  int id = *((int*)param);
  int numReaders = 0;

  for (int i = 0; i <= NUMBER_OF_READS; i++) {
    hold();
    pthread_mutex_lock(&m);

    READERS_WAITING_COUNTER++;
    while (READERS_COUNTER == -1) {
      pthread_cond_wait(&cond_reader, &m);
    }
    READERS_WAITING_COUNTER--;
    numReaders = ++READERS_COUNTER;
    pthread_mutex_unlock(&m);

    hold();
    printf("Reader %d: %d read by %d\n", id, SHARED_VALUE, numReaders);
    fflush(stdout);

    pthread_mutex_lock(&m);
    READERS_COUNTER--;
    if (READERS_COUNTER == 0) {
      pthread_cond_signal(&cond_writer);
    }
    pthread_mutex_unlock(&m);

    fflush(stdout);
  }

  return 0;
}

void *writer(void *param) {
  int id = *((int*)param);
  int numReaders = 0;

  for (int i = 0; i < NUMBER_OF_WRITES; i++) {
    hold();

    pthread_mutex_lock(&m);
    while (READERS_COUNTER != 0) {
      pthread_cond_wait(&cond_writer, &m);
    }

    READERS_COUNTER = -1;
    numReaders = READERS_COUNTER;
    SHARED_VALUE++;
    pthread_mutex_unlock(&m);

    hold();
    printf("Writer %d: %d read by %d\n", id, SHARED_VALUE, numReaders);
    fflush(stdout);

    pthread_mutex_lock(&m);
    READERS_COUNTER = 0;
    if (READERS_WAITING_COUNTER > 0) {
      pthread_cond_broadcast(&cond_reader);
    }
    else {
      pthread_cond_signal(&cond_writer);
    }
    pthread_mutex_unlock(&m);

    return 0;
  }
}
