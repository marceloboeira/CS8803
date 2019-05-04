# Lesson 6 - PThreads

* POSIX Threads - concreate multithread systems
* POSIX - Portable Operating System Interface
  * It describes and implements an interoperability API across different OSs programs.

## PThread Creation

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
