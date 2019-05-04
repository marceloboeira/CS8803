# Lesson 6 - PThreads

* POSIX Threads - concreate multithread systems
* POSIX - Portable Operating System Interface
  * It describes and implements an interoperability API across different OSs programs.

## PThread Creation

Functions available:
```c
// Type
pthread_t aThread; // type of a Thread

// Fork(proc, args)
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void * (*start_routine)(void *),
                   void *arg);

// Join
int pthread_join(pthread_t thread, void **status);


int pthread_create(&tid, &attr, foo, NULL)

```

* pthread_attr_t
  * specified in pthread_create
  * defines features of the new thread
    * stack size
    * joinable
    * priority
    * inheritance
    * scheduling policy
    * system/process scope
  * has default values, send `NULL` to use them

### Examples

```c
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
```

See `examples/simple_thread.c`.

### Quiz: PThread Creation Quiz 1

What is the output of the previous showned example?

```
Hello Thread
Hello Thread
Hello Thread
Hello Thread
```

### Example 2

```c
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
```

### Quiz: PThread Creation Quiz 2

What is the output of the previous shown example? (Check all that apply)

```
Thread number 0
Thread number 1
Thread number 2
Thread number 3
```

```
Thread number 0
Thread number 2
Thread number 1
Thread number 3
```

```
Thread number 0
Thread number 2
Thread number 2
Thread number 3
```

All are valid, because of 2 main things:

1. We can't guarantee the order on which the threads execute
2. We send a pointer to a variable i, which is global, meaning that changing it's value when dispatching the threads changes its value on all threads...

**Racing Condition**.

See `examples/thread_with_params.c`.


### Example 3 - Correctly sending params

```c
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
```

See `examples/thread_with_correct_params.c`.

* Now we don't pass the address of i, yet we send a specific array value, which is not changed along the code

### Quiz - PThread Creation Quiz 3

What is the output of the previous shown example? (Check all that apply)

```
Thread number 0
Thread number 0
Thread number 2
Thread number 3
```

```
Thread number 0
Thread number 2
Thread number 1
Thread number 3
```

```
Thread number 3
Thread number 2
Thread number 1
Thread number 0
```

We've fixed the thread number issue, so they don't repeat anymore. However, we still can't guarantee the order.

So, 1 is not possible, whereas 2 and 3 are.

## Mutexes

Functions available:
```c
pthread_mutex_t aMutex;

int pthread_mutex_lock(pthread_mutex_t *mut);
int pthread_mutex_unlock(pthread_mutex_t *mut);
int pthread_mutex_init(pthread_mutex_t *mut, const pthread_mutexattr_t *attr); // we must use this to allocate the mutex
int pthread_mutex_trylock(pthread_mutex_t *mut); // non-blocking
int pthread_mutex_destroy(pthread_mutex_t *mut); // free
```

## Condition Variables

Functions available:
```c
pthread_cond_t aCond;

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mut); // release the lock if not cond, goes idle until condition matches
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond); // free
```

## Producer and Consumer Example

Simplified:

```c
#define BUFFER_SIZE 3

int buffer[BUFFER_SIZE];  	/* shared buffer */
int add = 0; /* place to add next element */
int rem = 0; /* place to remove next element */
int num = 0; /* number elements in buffer */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  	/* mutex lock for buffer */
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

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

  return 0;
}

/* Produce value(s) */
void *producer(void *param) {
  int i;

  for (i = 1; i <= 10; i++) {
    pthread_mutex_lock(&m);
    while (num == BUFFER_SIZE) {
      pthread_cond_wait (&c_prod, &m);
    }

    /* if executing here, buffer not full so add element */
    buffer[add] = i;
    add = (add+1) % BUFFER_SIZE;
    num++;

    pthread_mutex_unlock(&m);
    pthread_cond_signal(&c_cons);
  }

  return 0;
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param) {
  int i;

  while (1) {
    pthread_mutex_lock(&m);

    while (num == 0) {  /* block if buffer empty */
      pthread_cond_wait(&c_cons, &m);
    }

    /* if executing here, buffer not empty so remove element */
    i = buffer[rem];
    rem = (rem+1) % BUFFER_SIZE;
    num--;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&c_prod);
  }

  return 0;
}
```
