# Operating System Laboratory ΓÇö Course Repository

Welcome to my official **Operating System Laboratory** repository. This repository contains structured solutions, C programs, shell scripts, and documentation for all lab assignments.

## ≡ƒæñ Student Information
- **Name**: Rishi
- **Roll Number**: 2024UCP1566
- **Branch**: Computer Science & Engineering
- **Institute**: Malaviya National Institute of Technology (MNIT), Jaipur
- **Repository**: [https://github.com/the-sage-00/OS-LAB](https://github.com/the-sage-00/OS-LAB)

---

## ≡ƒÄ» Repository Organization & Philosophy

I maintain this repository in a **disciplined, modular, and minimal way**:
1. **Clean Directory Structure**: Every assignment is organized into its own dedicated folder (`Lab1/`, `Lab2/`, etc.).
2. **Minimal & Final Files Only**: I keep only the final verified source code, scripts, and documentation. No intermediate temp build files or duplicate drafts.
3. **Comprehensive Documentation**: Each lab directory includes a detailed `README.md` explaining the concepts, theoretical answers, and step-by-step compilation/execution commands.

---

## ≡ƒôü Repository Sitemap

```text
OS-LAB/
Γö£ΓöÇΓöÇ README.md                  <- Main repository overview
Γöé
Γö£ΓöÇΓöÇ Lab1/                      <- Lab Assignment 1
Γöé   Γö£ΓöÇΓöÇ README.md              <- Overview of commands, shell, GCC, OSTEP API & Booting
Γöé   Γö£ΓöÇΓöÇ OS_Lab_2024UCP1566.docx<- Complete assignment report
Γöé   Γö£ΓöÇΓöÇ ostep_code/            <- C implementations of OSTEP CPU API (p1.c - p4.c)
Γöé   ΓööΓöÇΓöÇ screenshots/           <- Real terminal output logs & screenshots
Γöé
ΓööΓöÇΓöÇ Lab2/                      <- Lab Assignment 2
    Γö£ΓöÇΓöÇ README.md              <- Full answers, Q1 Notebook Guide & Q2-Q6 code details
    Γö£ΓöÇΓöÇ q2_proc_monitor.sh     <- /proc filesystem performance monitoring script
    Γö£ΓöÇΓöÇ q3_fork.cpp            <- Child process creation using fork() [C++]
    Γö£ΓöÇΓöÇ q4_aslr.sh             <- Shell script for enabling/disabling ASLR
    Γö£ΓöÇΓöÇ q5_variable_sharing.cpp<- Parent/Child variable sharing & memory address analysis [C++]
    Γö£ΓöÇΓöÇ q6_syscalls.cpp        <- Combined fork(), wait(), exit(), and execv() program [C++]
    ΓööΓöÇΓöÇ ostep_code/            <- OSTEP Chapter 2-5 demo code (finite loops) [C++]
        Γö£ΓöÇΓöÇ common.h
        Γö£ΓöÇΓöÇ cpu.cpp
        Γö£ΓöÇΓöÇ mem.cpp
        Γö£ΓöÇΓöÇ threads.cpp
        ΓööΓöÇΓöÇ io.cpp
```

---

## ≡ƒÜÇ Lab Summaries Quick Access

| Lab Folder | Focus Area | Key Concepts | Status |
|---|---|---|---|
| **[Lab1/](./Lab1)** | Linux Basics, Commands, GCC & Booting | Linux Commands, Shell, GCC flags, OSTEP Process API, Booting | Γ£à Completed |
| **[Lab2/](./Lab2)** | OSTEP Summaries, /proc, Fork, ASLR, COW | OSTEP Ch 1-4, /proc monitoring, Process Creation, ASLR, Memory Sharing | Γ£à Completed |

---

## ≡ƒ¢á∩╕Å How to Compile & Run Code

### Lab 2 Examples:
```bash
# Clone the repository
git clone https://github.com/the-sage-00/OS-LAB.git
cd OS-LAB/Lab2

# 1. Run /proc performance monitor
chmod +x q2_proc_monitor.sh
./q2_proc_monitor.sh

# 2. Compile and run Fork process demo (Q3)
g++ -o q3_fork q3_fork.cpp
./q3_fork

# 3. Check and manage ASLR (Q4)
chmod +x q4_aslr.sh
./q4_aslr.sh

# 4. Test Variable Sharing & Virtual Addresses (Q5)
g++ -o q5_variable_sharing q5_variable_sharing.cpp
./q5_variable_sharing

# 5. System Calls implementation (Q6)
g++ -o q6_syscalls q6_syscalls.cpp
./q6_syscalls
```
