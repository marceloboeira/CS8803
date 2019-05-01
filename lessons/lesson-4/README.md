# Lesson 4 - Processes and Process Management

## What's Covered?

* What is a Process?
* How does the OS represents a procees?
* How does the OS manager multiple concurrent processes?

## Simple Definition

A process is an instance of an executing program.

## Visual Metaphor

Order of toys:
- State of execution
- Parts & Temporary holding area
- May require special hardware

Process - Computation request:
- State of execution: Program Counter, Stack
- Parts & Temporary holding area: Data, register, state in memory
- May require special hardware: I/O Devices

## What is a Process?

Application: program on disk, not executing, static entity.

Once an application is loaded in memory and executing it's a **process**.

If an application is ran multiple times, we have multiple processes.

The OS Manages hardware on behalf of applications.

#### What does a process look like?

In memory, the process content and state must be identifiable by an address, on the addresspace,

````
vMax    Stack
          |
          |
          |
          |
          |
        Heap
        data -----|___ Static State
v0      text -----|
`````

#### Types of process state

* Static
  * text - code
  * data - files/content
* Dynamic
  * Stack
    * LIFO (last in first out)
    * grows and shrinks
    * Stores code addresses, state, and well-known side elements
  * Heap - Continuous space in memory for dynamic growing random-access data.

#### Process address space

v0 to vMax are virtual representations of physical addresses in memory used by the process.

They don't have to correspond to real physical addresses since this mapping is done by the hardware and the OS. This is due to interventions that the hardware of the OS can do in order to improve performance, security or reliability of the whole system, also compliant to multi-tasking (e.g.: when different processes are dynamically requiring different amounts of memory...).

PageTables are the mappings between the real addresses and the virtual ones, given to each process.

### Address Space and Memory Management

On a 32bits system, the addresses can only represent up to 4GB, that's one of the many reasons why the OS defines which portion of the phisycal address is stored and represented by which address on the virtual one.

e.g.:

Imagine that the P1 required 1 address, then it ran for a while, soon after the P2 started and it required 4 addresses, followed by another P1 request and finally P2 required 1 more.

See:
```
virtual -- phisycal
  P1         P1
  P1         P2
  P2         P2
  P2         P2
  P2         P1
  P2         P2
```

From the process perspective, on the virtual mapping, the space is continuous, since the OS calculates a gap for such events, whereas in reality the memory is not necessairly managed that way.

**Important** to notice that virtual address space is also useful for specifying which process can perform a specific memory action, since the mapping is done by the OS.

### Quiz: Virtual Addresses Quiz

If two processes, P1 and P2, are running at the same time, what are possible virtual address space ranges they'll be given?

- [ ] P1: 0      - 32,000 | P2: 32,001 - 64,000
- [x] P1: 0      - 64,000 | P2: 0      - 64,000
- [ ] P1: 32,001 - 0      | P2: 0      - 32.000

Since we've decoupled the addresses from the phisycal ones implies that both processes could have the same "virtual address", but in reality being pointed to different locations on the real hardware.

## Process Execution State
> How does the OS knows what a process is doing?

For the OS to manage the processes it must have a way to know what is the process doing. e.g.: if the OS stops the process due to multitasking, it must know how to re-start it from the exact same point.

Once the code is compiled, it becomes a binary, or a sequence of instructions (that are not necessairly sequential).

Since the CPU needs to know where your program is, the CPU watches the Program Counter, or PC, and the Registers.

State
* Program Counter (PC) - Address of the current instruction to be executed by the OS. (Maintained by the CPU in a register, along with other registers).
* Registers - address of the parameters of the instruction
* Stack Pointer (SP) - Address of the stack

### What is the Proccess Controll Block (PCB)?

The Process Controll Block is a reserved space in memory where the OS store information necessary for the process management.

* Process State
* Process Number
* Program Counter
* Registers
* Memory Limits
* List of Open Files
* Priority
* Signal Mask
* CPU Scheduling Info
* ...

The PCB is initializer when the process starts and it's updated during the lifetime of the process...

#### How is a PCB used?

When the process initializes the PC is set to the first instruction on the process code, the CPU sets the next ones on its register but it's the OS job to collect and update that on the PCB.

The CPU registers correspond to the values of the current "running" process. When the OS decides to interrupt a process, for whatever reason, it must save the state of the same, including CPU registers, PC, and such into the PCB.  When the process is rescheduled/restarted by the OS, the OS then must then update back the CPU registers with the process, as well as everything else the process might need (Program Counter, ...)

This swap is called **context-switch**.

### Context Switch

A context switch is the mechanism of the operating system to switch the execution from the context of one process to another process.

This operation is expensive, for several reasons:

* Direct cost:
  * Moving from/to memory to the CPU
* Indirect cost:
  * Cache misses / Cold cache

#### Cache misses?

The processor has a pool of different cache layers, L0, L1, L2... each bigger than the previous in space but slower in speed. This is created in order to facilitate the processing of instructions, since the path from L0 to the CPU is a couple orders of magnitude faster than if it is fetching the same values from memory.

When the context switch happens, it's quite likely that the new process will endup overrinding the existing cache, and then the previous process is back running it will have a cold cache, having to fetch again things from memory.

### Quiz: Hot Cache Quiz

When a cache is hot...

- [ ] it can malfunctions so we must context switch to another process
- [x] most process data is in the cache so the process performance will be at its best
- [x] sometimes we must context switch
