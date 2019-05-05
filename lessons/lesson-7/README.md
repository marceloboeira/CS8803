# Lesson 7 - Problem Set 1

## Priority Readers and Writers

Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable.

Essentially, if any readers are waiting, then they have priority over writer threads -- writers can only write when there are no readers.

This program should adhere to the following constraints:

* Multiple readers/writers must be supported (5 of each is fine)
* Readers must read the shared variable X number of times
* Writers must write the shared variable X number of times
* Readers must print:
  * The value read
  * The number of readers present when value is read
* Writers must print:
  * The written value
  * The number of readers present were when value is written (should be 0)
* Before a reader/writer attempts to access the shared variable it should wait some random amount of time
  * Note: This will help ensure that reads and writes do not occur all at once
* Use pthreads, mutexes, and condition variables to synchronize access to the shared variable

### Notes

* Quite unpleasant to use the readers -1 as a form of state, (easy to loose track of it)
* A bit weird to use the a `while` with a negative condition and then wait for the loop to go through and re-check the condition before executing the critical section

See `exercises/priority_readers_writers.c`.

## Simple Socket: Client

Write a simple C program that creates, initializes, and connects a client socket to a server socket. You should provide a way to specify the connecting server address and port. This can be hardcoded or passed via the command line.

See `exercises/socket_client.c`.
See `exercises/socket_client.rs`.

## Simple Socket: Server

Write a simple C program that creates and initializes a server socket. Once initialized, the server should accept a client connection, close the connection, and then exit.

See `exercises/socket_server.c`.
See `exercises/socket_server.rs`.
