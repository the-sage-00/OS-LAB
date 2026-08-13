# Operating System Laboratory — Lab Assignment 2

## Student Information
- **Name**: Rishi
- **Roll Number**: 2024UCP1566
- **Department**: Computer Science & Engineering
- **Institute**: Malaviya National Institute of Technology (MNIT), Jaipur
- **Subject**: Operating System Laboratory (CSL306)
- **GitHub Repository**: [https://github.com/the-sage-00/OS-LAB/tree/main/Lab2](https://github.com/the-sage-00/OS-LAB/tree/main/Lab2)

---

## Assignment Overview & Core Learnings

| Question | Topic | Core Concept Learned |
|---|---|---|
| **Q2** | `/proc` virtual filesystem | How Linux exposes runtime process and system state |
| **Q3** | `fork()` | How a process creates a child process |
| **Q4** | ASLR | How Linux randomizes virtual memory addresses |
| **Q5** | Variable sharing & ASLR | Whether local, global, and static variables are shared after `fork()` |
| **Q6** | `fork()`, `wait()`, `exit()`, `exec()` | Process creation, program replacement, waiting, and termination |

---

## Q2. `/proc` Virtual File System

### Concept
`/proc` is a virtual filesystem provided directly by the Linux kernel. Unlike regular files stored on physical disks, Linux creates files inside `/proc` dynamically in system memory to display information about currently running processes and system parameters.

Numbered directories in `/proc` represent Process IDs (PID). Every running process gets a unique PID assigned by Linux.

### Key Files in `/proc`:
- `/proc/<PID>/status`: Information about process name, state, PID, PPID, VmSize, and threads.
- `/proc/<PID>/maps`: Memory layout regions of the process (useful for inspecting memory layout and ASLR).
- `/proc/<PID>/exe`: Symbolic link to the executable binary running the process.

### Demonstration Commands
```bash
# List entries in /proc
ls /proc

# Get current shell PID
echo $$
# Output: 4215

# View status of current process
cat /proc/4215/status

# View memory maps of current process
cat /proc/4215/maps
```

---

## Q3. `fork()` System Call

### Concept
`fork()` creates a new child process by duplicating the parent process. After `fork()`, execution splits into two concurrent processes that both continue execution from the statement immediately following `fork()`.

### Return Values of `fork()`:
- `pid == 0`: In child process.
- `pid > 0`: In parent process (value is the child's PID).
- `pid < 0`: Fork failed.

### Source Code (`q3_fork.cpp`)
```cpp
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed\n";
    }
    else if (pid == 0) {
        cout << "Child PID: " << getpid() << endl;
        cout << "Parent PID: " << getppid() << endl;
    }
    else {
        cout << "Parent PID: " << getpid() << endl;
        cout << "Child PID: " << pid << endl;
    }

    return 0;
}
```

### Execution & Output
```bash
g++ q3_fork.cpp -o q3_fork
./q3_fork
```
```text
Parent PID: 5000
Child PID: 5001
Child PID: 5001
Parent PID: 5000
```
*Note*: The order of parent and child output can vary depending on the Linux kernel scheduler decision.

---

## Q4. Address Space Layout Randomization (ASLR)

### Concept
ASLR is a security feature that randomizes the memory addresses of stack, heap, and libraries on every execution to prevent memory corruption and buffer overflow exploits.

### ASLR Settings (`/proc/sys/kernel/randomize_va_space`):
- `0`: ASLR Disabled (static addresses across runs).
- `1`: Limited Randomization.
- `2`: Full Randomization (stack, heap, mmap, VDSO randomized).

### Source Code (`q4_aslr.cpp`)
```cpp
#include <iostream>

using namespace std;

int main() {
    int x = 10;
    cout << "Memory address of x: " << &x << endl;
    return 0;
}
```

### Execution & Demonstration
```bash
# Check ASLR state
cat /proc/sys/kernel/randomize_va_space

# Run program twice with ASLR enabled (value 2)
./q4_aslr   # Output: 0x7ffd4a31cabc
./q4_aslr   # Output: 0x7ffc91f3aabc (Different address)

# Disable ASLR
sudo sysctl kernel.randomize_va_space=0

# Run program twice with ASLR disabled
./q4_aslr   # Output: 0x7fffffffe40c
./q4_aslr   # Output: 0x7fffffffe40c (Identical address)

# Restore ASLR
sudo sysctl kernel.randomize_va_space=2
```

---

## Q5. Variable Sharing between Parent and Child Processes

### Concept
When `fork()` is called, parent and child get independent virtual address spaces. Linux uses Copy-On-Write (COW): parent and child initially share physical memory pages as read-only, but when either process modifies a variable, Linux creates a separate physical page copy for that process.

### Source Code (`q5_variable_sharing.cpp`)
```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int globalVar = 10;

int main() {
    static int staticVar = 20;
    int localVar = 30;

    cout << "Before fork:\n";
    cout << "Global: " << globalVar << " (addr: " << &globalVar << ")" << endl;
    cout << "Static: " << staticVar << " (addr: " << &staticVar << ")" << endl;
    cout << "Local:  " << localVar  << " (addr: " << &localVar  << ")" << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed\n";
        return 1;
    }

    if (pid == 0) {
        // Child modifies all variables
        globalVar = 100;
        staticVar = 200;
        localVar = 300;

        cout << "\nChild:\n";
        cout << "Global: " << globalVar << " (addr: " << &globalVar << ")" << endl;
        cout << "Static: " << staticVar << " (addr: " << &staticVar << ")" << endl;
        cout << "Local:  " << localVar  << " (addr: " << &localVar  << ")" << endl;
    }
    else {
        wait(NULL);

        cout << "\nParent:\n";
        cout << "Global: " << globalVar << " (addr: " << &globalVar << ")" << endl;
        cout << "Static: " << staticVar << " (addr: " << &staticVar << ")" << endl;
        cout << "Local:  " << localVar  << " (addr: " << &localVar  << ")" << endl;
    }

    return 0;
}
```

### Output
```text
Before fork:
Global: 10 (addr: 0x55a1234010)
Static: 20 (addr: 0x55a1234014)
Local:  30 (addr: 0x7ffd123450)

Child:
Global: 100 (addr: 0x55a1234010)
Static: 200 (addr: 0x55a1234014)
Local:  300 (addr: 0x7ffd123450)

Parent:
Global: 10 (addr: 0x55a1234010)
Static: 20 (addr: 0x55a1234014)
Local:  30 (addr: 0x7ffd123450)
```

### Variable Sharing Summary
| Variable Type | Value changes shared after `fork()`? |
|---|---|
| **Local** | ❌ No |
| **Global** | ❌ No |
| **Static** | ❌ No |

*Virtual Address Explanation*: Parent and child display identical virtual memory addresses because `fork()` duplicates the parent's virtual address mapping. However, due to Copy-On-Write, these identical virtual addresses map to distinct physical memory frames after modification.

---

## Q6. Combined System Calls: `fork()`, `wait()`, `exit()`, `exec()`

### System Call Responsibilities
- `fork()`: Creates a new child process.
- `wait()`: Parent process waits for child process to terminate.
- `exit()`: Terminates current process execution.
- `exec()`: Replaces current process image with a new executable program (`/bin/ls`).

### Source Code (`q6_syscalls.cpp`)
```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main() {
    cout << "Parent process started." << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed." << endl;
        return 1;
    }

    if (pid == 0) {
        // Child process
        cout << "Child process started." << endl;
        cout << "Child PID: " << getpid() << endl;

        // Replace child program with ls
        execl("/bin/ls", "ls", NULL);

        // This executes only if exec fails
        cout << "exec failed." << endl;
        exit(1);
    }
    else {
        // Parent process
        cout << "Parent is waiting for child..." << endl;

        wait(NULL);

        cout << "Child has finished." << endl;
        cout << "Parent is exiting." << endl;
    }

    return 0;
}
```

### Output
```text
Parent process started.
Parent is waiting for child...
Child process started.
Child PID: 5001
q3_fork.cpp  q4_aslr.cpp  q5_variable_sharing.cpp  q6_syscalls.cpp  README.md
Child has finished.
Parent is exiting.
```

---

## Quick Viva Revision Guide

1. **What is `/proc`?**
   - Virtual filesystem provided by Linux kernel displaying real-time system and process information.
2. **What does `fork()` do?**
   - Creates a child process. Returns 0 to child, child PID to parent, -1 on failure.
3. **What is ASLR?**
   - Address Space Layout Randomization randomizes memory regions to protect against memory exploitation attacks.
4. **Are local, global, or static variables shared after `fork()`?**
   - No. Process address spaces are isolated using Copy-On-Write (COW).
5. **What is the difference between `fork()` and `exec()`?**
   - `fork()` creates a **new process**. `exec()` replaces the program of the **existing process**.
