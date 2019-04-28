# Lesson 3 - Introduction to Operating Systems

## What's Covered?

* What is an Operating System?
* What is the role that it plays in Computer Systems?
* What are the key components?
* Design and implementaion of consideration of operating systems

## Simple OS Definition

Operating System - a piece of software that **abtracts** and **arbitrates** the underlying hardware system.

Abstract - to simplify what the hardware does, to make it generic across different types of hardware.
Arbitrate - to manage, to oversee details, to control the hardware use.

## Visual Metaphor

"An OS is like ... a toy shop manager":

* Directs Operational Resources
  * Controlling employee time
  * Which employee works on each task
  * Tools...
* Enforces Policies
  * Fairness - how employees have to share resources fairly when working on similar tasks
  * Clean-up - how to clean up after a task is finished
* Mitigate difficulty of complex tasks
  * Simplify operations
  * Optimize performance

Whereas an OS:

* Directs Operational Resources
  * Controls CPU, memory, peripherical devices...
* Enforces Policies
  * Fair resource access (e.g.: CPU, Memory, Disk...)
  * Set limits (e.g.: number of files open, threads, ...)
* Mitigate difficulty of complex tasks
  * Abstract hardware details (e.g.: system calls for memory management)

## What's an Operating System?

A layer of system software between applications and hardware that abstracts and arbitrates common tasks.

It:
* Has privileged access to the hardware
* Hides complexity (hardware specific)
* Manages hardware on behalf of one or more applications (according to policies)
* Provides isolation and protection between applications

```
                      Applications
    Internet Browser                   Text Editor
----------|---------------------------------|-------------
          |         Operating System        |
      send/recv                      Read/Write Files
     Network Socket                         |
          |            _____________________|
          |            |
          L____________|________
                       |       |
Scheduling--|          |       |
---|--------|----------|-------|--------------------------
  CPU   |  GPU  |  Memory  | Disk | Ethernet | USB Drives
````

### Quiz: Components, Abstraction, Arbitration

#### Which of the following are likely components of an operating system?

- [ ] File editor   (it's an application)
- [x] File system   (it abstracts blocks of disk storage into files)
- [x] Device driver (it abstracts decisions of the hardware device)
- [ ] Cache memory  (the OS doesn't manages it, the hardware does)
- [ ] Web browser   (it's an application)
- [x] Scheduler     (it's abstract CPU resources, pushing and pullling tasks that need computation)

#### Abstraction or Arbitration?
> [x] Abstraction | [ ] Arbitration

- [ ] distributing memory between multiple processes
- [x] supporting different types of speakers
- [x] interchangeable access to different types of HD or SSD

### Types of Operating Systems

Each OS is designed for different types of usage, that being a desktop, a laptop, a mobile phone, up to a server machine, or even a IoT device.

The focus of the classes is Desktop, with some bits of embedded.

## OS Elements

OS supports a number of higher level **abstractions**, such as:
 * Process
 * Thread
 * File
 * Socket
 * Memory Page

As well as a number of key **mechanisms** that operate on top of these instructions:
 * Create, Schedule (applications, e.g.: processes & threads)
 * Open, Write (Disk blocks/Files)
 * Allocate (Memory)

Finally, they also support **policies** to determine specifically how these will be used:
 * LRU (least recently used)
 * EDF (earliest deadline first)
