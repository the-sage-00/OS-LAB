# Lab 2: OSTEP Summaries, /proc File System, Process Creation, ASLR & Variable Sharing

**Student Name**: Rishi  
**Roll Number**: 2024UCP1566  
**Course**: Operating Systems Laboratory  
**Institute**: Malaviya National Institute of Technology (MNIT), Jaipur  

---

## 📌 Table of Contents
1. [Question 1: OSTEP Chapter Summaries (Notebook Content)](#question-1-ostep-chapter-summaries-notebook-content)
2. [Question 2: /proc Performance Monitoring Script](#question-2-proc-performance-monitoring-script)
3. [Question 3: Child Process Creation using fork()](#question-3-child-process-creation-using-fork)
4. [Question 4: Enabling and Disabling ASLR](#question-4-enabling-and-disabling-aslr)
5. [Question 5: Variable Sharing & Address Analysis (ASLR Impact)](#question-5-variable-sharing--address-analysis-aslr-impact)
6. [Question 6: System Calls Implementation (fork, wait, exit, execv)](#question-6-system-calls-implementation-fork-wait-exit-execv)

---

## Question 1: OSTEP Chapter Summaries (Notebook Content)

> **Note for Notebook Writing**: Write the content below in your physical notebook. Keep it clean and underline the **bold** key terms.

### Chapter 1: Introduction to Operating Systems

#### 1.1 Overview of Operating Systems
An **Operating System (OS)** is a set of system software that acts as an intermediary between user applications and computer hardware.

The primary responsibilities of an OS are built upon three core pillars:
1. **Virtualization**: Transforming physical hardware (CPU, Memory, Disk) into easy-to-use virtual forms.
2. **Concurrency**: Managing multiple tasks executing simultaneously without data corruption.
3. **Persistence**: Storing data permanently on non-volatile storage devices.

#### 1.2 Virtualizing the CPU
The OS creates the **illusion** that the system has an infinite number of CPUs.
- It uses a technique called **time-sharing**, where the OS runs one process for a short time slice, pauses it, and switches to another process.
- This rapid switching allows multiple programs to run concurrently on a single physical CPU core.

#### 1.3 Virtualizing Memory
Each process is provided with its own private **virtual address space**.
- The OS maps virtual memory addresses used by a program to physical memory (RAM) addresses.
- Memory isolation ensures that one process cannot access or corrupt the memory space of another process.

#### 1.4 Concurrency Issues
Modern software uses **threads** to perform multiple tasks within the same memory space.
- Uncontrolled concurrent execution leads to **race conditions** when updating shared variables.
- The OS provides synchronization mechanisms such as **locks**, **semaphores**, and **condition variables** to ensure data consistency.

#### 1.5 Persistence & File Systems
Primary memory (RAM) is **volatile** and loses data when powered off.
- The OS provides a **file system** abstraction (files and directories) stored on non-volatile devices (HDD/SSD).
- Programs interact with persistent storage using system calls like `open()`, `read()`, `write()`, and `close()`.

#### 1.6 Design Goals of an OS
1. **Abstraction**: Hide low-level hardware details behind simple, clean APIs.
2. **Performance**: Minimize OS execution overhead.
3. **Protection**: Isolate processes from one another to prevent crashes from affecting the entire system.
4. **Reliability**: Ensure uninterrupted system operation over long periods.

---

### Chapter 2: Processes

#### 2.1 Concept of a Process
A **process** is defined as an **instance of a running program**.
- A program on disk is a passive entity (static instructions and data).
- A process is an active entity with a program counter, registers, and memory allocations.

#### 2.2 Machine State of a Process
To execute and manage a process, the OS maintains its **machine state**:
1. **Address Space**:
   - **Code Segment**: Machine instructions of the program.
   - **Data Segment**: Global and static variables.
   - **Heap Segment**: Dynamically allocated memory (`malloc`).
   - **Stack Segment**: Function call frames, local variables, and return addresses.
2. **Register Context**:
   - **Program Counter (PC)**: Points to the next instruction to execute.
   - **Stack Pointer (SP)**: Points to the top of the stack.
3. **I/O Information**: List of currently open file descriptors.

#### 2.3 Process States
A process transitions between three main states:
1. **Running**: Process is actively executing instructions on the CPU core.
2. **Ready**: Process is prepared to run, waiting for the OS scheduler to allocate CPU time.
3. **Blocked**: Process is waiting for an external event (e.g., I/O completion) and cannot run.

#### 2.4 Process Control Block (PCB)
The OS tracks each process using a data structure called the **Process Control Block (PCB)** or process descriptor. It contains:
- **Process ID (PID)**: Unique numerical identifier.
- **Process State**: Running, Ready, or Blocked.
- **Saved Registers**: CPU register state saved during context switches.
- **Memory Management Information**: Page tables and bounds.
- **Accounting & I/O Status**: Open files and CPU time used.

#### 2.5 Context Switching
A **context switch** is the low-level mechanism used by the OS to switch CPU execution from one process to another:
1. Save the register state of the currently running process into its PCB.
2. Restore the register state of the target process from its PCB.
3. Switch stack pointers and resume execution at the restored Program Counter.

---

### Chapter 3: Process API

#### 3.1 `fork()` System Call
The `fork()` system call creates a new process by duplicating the calling (parent) process.
- The new process is called the **child process**.
- Both processes continue execution from the statement immediately following `fork()`.
- Return values of `fork()`:
  - Returns `0` to the **child process**.
  - Returns the **child's PID** to the **parent process**.
  - Returns `-1` on **failure**.

#### 3.2 `wait()` System Call
The `wait()` or `waitpid()` system call suspends the parent process until one of its child processes terminates.
- Without `wait()`, the order of execution between parent and child is **non-deterministic** (controlled entirely by the OS scheduler).
- `wait()` guarantees that the parent executes *after* the child has finished.

#### 3.3 `exec()` System Call Family
The `exec()` family (`execv`, `execvp`, `execl`, etc.) replaces the current process image with a new program image.
- It loads executable code from disk into the current process's address space.
- Overwrites code, heap, and stack segments.
- Does **not** create a new PID; execution continues as the new program.

#### 3.4 Why Separate `fork()` and `exec()`?
Separating process creation (`fork`) from program execution (`exec`) gives the shell the opportunity to modify the child's environment (e.g., redirecting file descriptors `>`) before running the command.

#### 3.5 `exit()` System Call
The `exit()` system call terminates the calling process immediately.
- Releases allocated memory, closes open file descriptors, and returns a exit status byte to the parent.

---

### Chapter 4: Process Termination

Processes terminate under two conditions:
1. **Normal Termination**: The process completes its `main()` function or explicitly calls `exit(status)`.
2. **Abnormal Termination**: The process receives a fatal signal (e.g., SIGSEGV due to segmentation fault, or SIGKILL).

#### Special Termination Cases:
- **Zombie Process**: A child process that has terminated, but whose parent has not yet called `wait()`. Its entry remains in the process table to hold the exit status.
- **Orphan Process**: A child process whose parent process has terminated. Orphan processes are automatically adopted by the `init` or `systemd` process (PID 1), which cleans them up when they finish.

---

## Question 2: /proc Performance Monitoring Script

### Description
The `/proc` virtual filesystem is a pseudo-filesystem provided by the Linux kernel. It acts as an interface to internal kernel data structures and system runtime statistics.

### Shell Script (`q2_proc_monitor.sh`)
```bash
./q2_proc_monitor.sh 2 10
```
This script reads `/proc/stat`, `/proc/meminfo`, and `/proc/uptime` to measure real-time CPU utilization percentage, available memory, active process counts, and system uptime.

---

## Question 3: Child Process Creation using `fork()`

### Source Code (`q3_fork.c`)
Demonstrates process duplication using `fork()`.

### Compilation & Execution:
```bash
gcc -o q3_fork q3_fork.c
./q3_fork
```

### Output:
```
=== Question 3: Fork System Call Demo ===
Parent Process Started (PID: 1234)

[CHILD PROCESS]
  My PID          : 1235
  My Parent PID   : 1234
  fork() return   : 0
Child process exiting gracefully...

[PARENT PROCESS]
  My PID          : 1234
  Created Child   : 1235
  fork() return   : 1235
Parent confirmed child (PID: 1235) exited with status: 0
Main program ended cleanly.
```

---

## Question 4: Enabling and Disabling ASLR

### What is ASLR?
**Address Space Layout Randomization (ASLR)** is a security feature that randomizes the memory addresses of key data areas (stack, heap, libraries) to defend against memory corruption attacks like buffer overflows.

### Kernel Configuration File
`/proc/sys/kernel/randomize_va_space`

### ASLR Modes:
- `0`: Disabled.
- `1`: Conservative randomization (Stack, VDSO, mmap).
- `2`: Full randomization (Stack, VDSO, mmap, and Heap/brk).

### Commands:
```bash
# Check current status
cat /proc/sys/kernel/randomize_va_space

# Disable ASLR (Requires root)
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

# Enable Full ASLR (Requires root)
echo 2 | sudo tee /proc/sys/kernel/randomize_va_space

# Disable ASLR for a single command execution without changing global setting:
setarch $(uname -m) -R ./program
```

---

## Question 5: Variable Sharing & Address Analysis (ASLR Impact)

### Code (`q5_variable_sharing.c`)
Tests whether Global, Static, and Local variables are shared between parent and child processes.

### Observations & Key Takeaways:
1. **Are variables shared?** **NO.** Modifying variables in the child process does NOT change their values in the parent process.
2. **Why are virtual addresses identical?** Because `fork()` creates an exact duplicate of the virtual address space layout.
3. **Copy-On-Write (COW)**: Physical memory pages are shared read-only initially. When child or parent modifies a variable, the OS kernel allocates a new physical memory page for that process.
4. **ASLR Impact**:
   - **ASLR Enabled**: Virtual addresses vary across different program runs.
   - **ASLR Disabled**: Virtual addresses remain identical across multiple program runs.

---

## Question 6: System Calls Implementation (fork, wait, exit, execv)

### Source Code (`q6_syscalls.c`)
Combines `fork()`, `wait()`, `exit()`, and `execv()` into a complete program flow.

### Execution Output:
```
======================================================================
     Question 6: Demonstration of fork(), wait(), exit(), execv()     
======================================================================
Parent Process (PID: 2040) starting...

[PARENT PROCESS] PID: 2040 created Child PID: 2041
[PARENT PROCESS] Waiting for child process to complete...
[CHILD PROCESS] PID: 2041
[CHILD PROCESS] Preparing arguments for execv()...
[CHILD PROCESS] Calling execv('/bin/ls', args)...
----------------------------------------------------------------------
total 64K
-rwxrwxrwx 1 saini saini 16K Aug  7 17:32 q3_fork
-rwxrwxrwx 1 saini saini 16K Aug  7 17:32 q5_variable_sharing
-rwxrwxrwx 1 saini saini 16K Aug  7 17:32 q6_syscalls

----------------------------------------------------------------------
[PARENT PROCESS] Child PID 2041 terminated.
[PARENT PROCESS] Child exited normally with status: 0
======================================================================
```
