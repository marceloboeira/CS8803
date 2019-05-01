# Lesson 5 - Threads and Concurrency

## What's covered?

* What are threads?
* What's concurrency?
* How theads different from processes?
* What data structures are important to use and manage threads?

References:
* ["An Introduction to Programming with Threads"](https://s3.amazonaws.com/content.udacity-data.com/courses/ud923/references/ud923-birrell-paper.pdf) by Birrell

## Simple Definition

A thread is an instance of an executing program.

## Visual Metaphor

Toy shop worker:
- Active entity - executing a unit of toy order
- Simultaneously with others - many workers completing toy order
- Requires coordination - shares tools, workstations...

Thread - :
- Active entity - executing a unit of a process
- Simultaneously with others - many threads executing at the same time
- Requires coordination - sharing of I/O devices CPUs, memory...

## What is a Thread?

Threads represent multiple independent execution contexts, they share the same virtual address space, code, data, files. However, they might execute different things on different orders... so they need different PC, Stack and Registers. That's represented by a more complex PCB, having a shared state and per-thread execution context.

### Why are threads useful?

Divide to conquer!

Imagine a 4 CPU computer, where a vector with 1M entries needs to be multiplied, on each element by 5. We can split the vector in 4 parts of 250k elements and run that in parallel on each CPU/Thread.

FASTER RESULTS! Probably 4 times faster in this case, even more if you have "waiting" state in between.
```
 T0   T1   T2   T3
CPU0 CPU1 CPU2 CPU3
```

Also, we can prioritize different threads, and each thread can have processor cache for their own operations.

Why not simply use multi-process?

* IPC is more costly than L* cache
* Having multiple processes generates memory overhead (in space)
* More complex both on OS level and on code

Are threads useful on a Single-CPU? or when the number of threads is bigger than the number of CPUs?

* Yes, when you have IO for instance, the IO/idle thread.
* Threads are faster because the context switch doesn't have to recreate the mappingings for addresses ans such
* It allows us to hide latency on IO operations

### Quiz: Process vs Threads Quiz

Does it apply to P - Process, T - Threads, B - Both.

- [T] can share a virtual address space
- [P] take longer to context switch
- [B] have an execution context
- [T] usually result in hotter cache when multiple exist
- [B] make use of some communication mechanism

## Basic Thread Mechanisms

* Thread Data Structure
  * Identify and keep track of thread resource usage
* Create / Manage Threads
* Coordinate memory access
  * Guarantees that are no conflicts since they're on the same address space

The OS with the PCB manages to create isolation across process memory space, P1 cannot change memory of P2. When we are talking about threads, that execute concurrently, we need mechanisms to make sure that T1 and T2 do not have race conditions for memory entries.

One thread might try to read the data while another is writing, and that can leads to faulty results.

### Synchronizations Mechanisms

* Mutual Exclusion
  * **Mutex** - Mutual exclusion is one mechanism that allows exclusive resource access to one thread at a time.
* Condition Variables
  * Control variables to define if a thread must be executed taking into account other threads.

## Threads Creation (Theory)

* Thread Type
  * Thread ID, PC, Register, Stack Pointer, data, attributes (for scheduling purposes)
  * Creation
    * fork(proc, args)
      * Not UNIX fork
      * T0 forks to T1, the "PCB" of the thread is copied and the PC is set to the same instruction and they execute concurrently.
      * When T1 finishes, it can write the result to memory, but how do we know it is finished?
    * join(threads)
      * we need to guarantee that the main thread ("owner" of the process) won't finish before the other threads do, so we can ensure their work is done.
      * when we call join, the executing thread waits until the threads given as argument are finished, so we ensure the above mentioned.

#### Example of thread creation

```
Thread thread1;
SharedList list;
thread1 = fork(safe_insert, 4);
safe_insert(6);
join(thread1); //Optional
```

```
     t0
     |
  t1 = fork... --------- t1
     |                    |
     |               safe_insert(4)
  safe_insert(6)
     |
   join(...)
```

There is no way to ensure, on the above scenario, that safe_insert 6 will run before 5.
The list can be either

* `4->6->nil`
* `6->4->nil`

The join only ensures that safe_insert 4 runs, if already ran it return immediately.

## Mutexes

### Problem: How is the list updated?

```
list --> value|p_next
```
the list is composed of a series of `values` and `pointer` tuples. When the list has to be updated, a thread takes the pointer and points to a new value. If a thread is to be updating at the same time as the other, it could generate a state where instead of 2 new elements, we could have 1 only. They might both read the same value of the list for `p_next` and then take turn setting the value of the last one... 1 get's inserts 1 is lost.

Mutex helps to solve this, by saving a "lock" flag, making it exclusive to the owner thread.

The mutex stores also the owner and a ordered list of blocked threads. The blocked threads have to wait.

e.g.:

Birrel's API:
````
lock(mutex) {
  mutex.counter += 1
} // lock releases
````

Common C API
````
lock(mutex)
mutex.counter += 1
unlocl(mutex)
````

### Mutex Example

```c
list<int> my_list;
Mutex m;

void safe_insert(int i) {
  Lock(m) {
    my_list.insert(i)
  } //unlock
}
```


### Quiz: Mutex Quiz

<img src="quiz_mutex.png" width="400">

We have 5 threads, T1-5, and the have 1 mutex.

T1 is the first one to get access, which thread gets the lock, right after T1 releases it?

- [x] T2 - it's request is already waiting for the lock, it will get it
- [ ] T3 - Not a likely candidate, since T5 is timeline-wise coming first
- [x] T4 - it's request is already waiting for the lock, it will get it
- [x] T5 - T5 can lock the request on another CPU even before T2/T4 can get a change to do it

## Producer and Consumer Example

What it the mutual exclusion only needs to happen under certain conditions... say for instance that any thread can read the list but only a single thread at a time can write to it...

```
Producer | ---> | 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 | Consumer
                  |                |_________________|
Producer |--------|
```

```
for i=0..10
  producers[i] = fork(safe_insert, NULL) //create producers

consumer = fork(print_and_clear, my_list) //create consumer

//producer insert
lock(m) {
   list->insert(my_thread_id)
}

//consumer print_and_clear
lock(m) {
  if my_list.full -> print; //-> ensures the other threads are finished by checking if it's full
  else -> release and try again (later) // return the execution control, or the lock, to the producer threads
}
```

The above code, is wasteful.

### Condition Variable

We can adjust the previous code to:

```
lock(m) {
   list->insert(my_thread_id)
}
```
