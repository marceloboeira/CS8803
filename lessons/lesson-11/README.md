# Lesson 11 - Scheduling

## OS Scheduler

Schedules work for the CPU.

The scheduler could:

* dispatch immediately
  * FIFO
  * Simple Scheduling
  * No Overhead
* dispatch simle orders first
  * Maximize * of Orders
  * More analysis during scheduling
  * More overhead
* dispatch complex orders first
  * Maximize resource usage
  * keep "CPU busy"

### CPU Scheduling

<img src="cpu.png" width="400">

* The scheduler moves things from the ready queue to the CPU as it sees fit.
* Tasks could execute and be dispatched to run IO, became ready to "continue" after IO (being scheduled again and such).
* Whenever the CPU is idle we run the scheduler, it looks at all tasks and decides which one makes sense to be scheduled next.

#### Timeslice

* Time is the biggest resource of a CPU
* One way that the scheduling could go is that each task has a amount of time on the CPU to run
  * Once the task timeout it's reservation, the scheduler interrupts it and pushes another task to run...

## Types of Scheduling

### Run To Completion Scheduling

Imagine we have a poll of tasks, that we know how long it should take, and we have only a single CPU.

Metrics analysed
* throughput
* avg time for task completion
* avg wait time
* overall cpu utilization

* FCFS - First-Come First-Serve
  * Schedules Tasks in Order of Arrival
  * A new tasks goes to the end of the queue
  * Scheduler consumes from the start of the queue
  * Data Scructure: FIFO
  * Example: T1=1s, T2=10s, T3=1s
    * throughput = 3/12s = 0.25 tasks/s
    * avg time for task completion = (1 + (10 + 1) + (1 + 10 + 1)) / 3 = 8s
    * avg wait time = (0 + 1 + 11) / 3 = 4s
    * overall cpu utilization

* SJF - Shortest Job First
  * Schedules Tasks in Order of Execution Time (Needs to know in advance)
  * Data Scructure: ordered FIFO or a Tree
  * Example: T1=1s, T2=10s, T3=1s
    * Order of Execution => T1 => T3 => T2
    * throughput = 3/12s = 0.25 tasks/s
    * avg time for task completion = (1 + (1 + 1) + (2 + 10)) / 3 = 5s
    * avg wait time = (0 + 1 + (1 + 1) / 3 = 1s
    * overall cpu utilization

### Quiz: SJF Performance Quiz

Basically, do the metrics evaluation for the SJF alg:

* throughput = 3/12s = 0.25 tasks/s
* avg time for task completion = (1 + (1 + 1) + (2 + 10)) / 3 = 5s
* avg wait time = (0 + 1 + (1 + 1) / 3 = 1s
* overall cpu utilization

### Preemptive Scheduling

Runtime is an assumption based on history or complexity.

<img src="prio.png" width="600">

Priority Scheduling
* Tasks from the kernel, essential to OS, could have different priority
* Priority is more important than run time
* Multiple runqueues (one for each prio level)

<img src="prio2.png" width="400">

* Danger: Starvation
  * Too many high-prio tasks, no time to execute low-prio, leading to no tasks being executed
  * Solution:
    * Take into account aging, the longer a tasks spends on the queue the higher the prio becomes.
    * That prevents tasks from marinating too long in the queue, since they gain higher prio quickly enough.

## Quiz: Preemptive Scheduling

Given the values shown on the table, complete the finishing times for each times.

`P3 < P2 < P1`

* T1 Finishes at: 8s
* T2 Finishes at: 10s
* T3 Finishes at: 11s

### Priority Inversion (SFJ)

When dependencies within the task execution is not orchestrated properly, since the lowest prio runs first but acquires a resource (e.g.: Lock) leading the high-prio task to go to a wait queue and give back the execution to the low-prio which finishes first, because it's the only runnable task.

Solution: temp boost for mutex owners. That helps to avoid intermediate prio tasks, hacking through the inversion.

## Round Robin Scheduling

3 tasks, they all have the same prio, they all show up at the same time.

* Gets the first task of the queue FCFS style
* Tasks do not run to completion, they either yield or get interrupted
* The tasks are scheduled like FCFS but might include priority, always traversing the queue T1-T2-T3, getting the next higher prio, not necessairly the highest prio of all...

More on Timeslicing next.

## Timeslices

* Maximum amount of uninterrupted CPU time given to a task. aka time quantum.
* Tasks may run less than a timeslice time if:
  * waits for IO (it gets placed on a queue)
  * if a higher-prio task gets scheduled
  * ...
* Using timeslices tasks are interleaved to ensure timesharing among other CPU tasks
