# Operating System Laboratory — Course Repository

Welcome to my official **Operating System Laboratory** repository. This repository contains structured solutions, C programs, shell scripts, and documentation for all lab assignments.

## 👤 Student Information
- **Name**: Rishi
- **Roll Number**: 2024UCP1566
- **Branch**: Computer Science & Engineering
- **Institute**: Malaviya National Institute of Technology (MNIT), Jaipur
- **Repository**: [https://github.com/the-sage-00/OS-LAB](https://github.com/the-sage-00/OS-LAB)

---

## 🎯 Repository Organization & Philosophy

I maintain this repository in a **disciplined, modular, and minimal way**:
1. **Clean Directory Structure**: Every assignment is organized into its own dedicated folder (`Lab1/`, `Lab2/`, etc.).
2. **Minimal & Final Files Only**: I keep only the final verified source code, scripts, and documentation. No intermediate temp build files or duplicate drafts.
3. **Comprehensive Documentation**: Each lab directory includes a detailed `README.md` explaining the concepts, theoretical answers, and step-by-step compilation/execution commands.

---

## 📁 Repository Sitemap

```text
OS-LAB/
├── README.md                  <- Main repository overview
│
├── Lab1/                      <- Lab Assignment 1
│   ├── README.md              <- Overview of commands, shell, GCC, OSTEP API & Booting
│   ├── OS_Lab_2024UCP1566.docx<- Complete assignment report
│   ├── ostep_code/            <- C implementations of OSTEP CPU API (p1.c - p4.c)
│   └── screenshots/           <- Real terminal output logs & screenshots
│
└── Lab2/                      <- Lab Assignment 2
    ├── README.md              <- Full answers, Q1 Notebook Guide & Q2-Q6 code details
    ├── q2_proc_monitor.sh     <- /proc filesystem performance monitoring script
    ├── q3_fork.c              <- Child process creation using fork()
    ├── q4_aslr.sh             <- Shell script for enabling/disabling ASLR
    ├── q5_variable_sharing.c  <- Parent/Child variable sharing & memory address analysis
    ├── q6_syscalls.c          <- Combined fork(), wait(), exit(), and execv() program
    └── ostep_code/            <- OSTEP Chapter 2-5 demo code (finite loops)
        ├── common.h
        ├── cpu.c
        ├── mem.c
        ├── threads.c
        ├── io.c
        ├── p1.c
        ├── p2.c
        ├── p3.c
        └── p4.c
```

---

## 🚀 Lab Summaries Quick Access

| Lab Folder | Focus Area | Key Concepts | Status |
|---|---|---|---|
| **[Lab1/](./Lab1)** | Linux Basics, Commands, GCC & Booting | Linux Commands, Shell, GCC flags, OSTEP Process API, Booting | ✅ Completed |
| **[Lab2/](./Lab2)** | OSTEP Summaries, /proc, Fork, ASLR, COW | OSTEP Ch 1-4, /proc monitoring, Process Creation, ASLR, Memory Sharing | ✅ Completed |

---

## 🛠️ How to Compile & Run Code

### Lab 2 Examples:
```bash
# Clone the repository
git clone https://github.com/the-sage-00/OS-LAB.git
cd OS-LAB/Lab2

# 1. Run /proc performance monitor
chmod +x q2_proc_monitor.sh
./q2_proc_monitor.sh 2 10

# 2. Compile and run Fork process demo (Q3)
gcc -o q3_fork q3_fork.c
./q3_fork

# 3. Check and manage ASLR (Q4)
chmod +x q4_aslr.sh
./q4_aslr.sh

# 4. Test Variable Sharing & Virtual Addresses (Q5)
gcc -o q5_variable_sharing q5_variable_sharing.c
./q5_variable_sharing

# 5. System Calls implementation (Q6)
gcc -o q6_syscalls q6_syscalls.c
./q6_syscalls
```
