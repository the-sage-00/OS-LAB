/*
 * OS Lab Assignment 2 — Question 5: Variable Sharing & Virtual Memory Address Analysis [C++]
 * Tests whether Global, Static, and Local variables are shared between Parent and Child.
 * Prints virtual memory addresses and values before and after modification in child.
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 1. Global Variable (Stored in Data/BSS Segment)
int global_var = 100;

int main() {
    // 2. Static Variable (Stored in Data Segment)
    static int static_var = 200;

    // 3. Local Variable (Stored on Stack Segment)
    int local_var = 300;

    std::cout << "======================================================================\n";
    std::cout << "        Question 5: Parent-Child Variable Sharing & Address Test [C++] \n";
    std::cout << "======================================================================\n";
    std::cout << "[BEFORE FORK] Process PID: " << getpid() << "\n";
    std::cout << "  global_var  = " << global_var << "  at Address: " << (void*)&global_var << "\n";
    std::cout << "  static_var  = " << static_var << "  at Address: " << (void*)&static_var << "\n";
    std::cout << "  local_var   = " << local_var  << "  at Address: " << (void*)&local_var  << "\n";
    std::cout << "----------------------------------------------------------------------\n";

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork failed!" << std::endl;
        return 1;
    } 
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        std::cout << "\n[CHILD PROCESS] PID: " << getpid() << " (Parent PID: " << getppid() << ")\n";
        std::cout << "  Initial values & addresses in Child:\n";
        std::cout << "    global_var  = " << global_var << "  at Address: " << (void*)&global_var << "\n";
        std::cout << "    static_var  = " << static_var << "  at Address: " << (void*)&static_var << "\n";
        std::cout << "    local_var   = " << local_var  << "  at Address: " << (void*)&local_var  << "\n";

        std::cout << "\n  --> Child modifying variables: +50 to each...\n";
        global_var += 50;
        static_var += 50;
        local_var  += 50;

        std::cout << "  Values after modification in Child:\n";
        std::cout << "    global_var  = " << global_var << "  at Address: " << (void*)&global_var << "\n";
        std::cout << "    static_var  = " << static_var << "  at Address: " << (void*)&static_var << "\n";
        std::cout << "    local_var   = " << local_var  << "  at Address: " << (void*)&local_var  << "\n";
        
        std::cout << "Child process exiting...\n";
        exit(0);
    } 
    else {
        // --- PARENT PROCESS ---
        // Wait for child to complete its modifications first
        wait(NULL);

        std::cout << "\n[PARENT PROCESS] PID: " << getpid() << "\n";
        std::cout << "  Values & addresses in Parent after Child finished:\n";
        std::cout << "    global_var  = " << global_var << "  at Address: " << (void*)&global_var << "\n";
        std::cout << "    static_var  = " << static_var << "  at Address: " << (void*)&static_var << "\n";
        std::cout << "    local_var   = " << local_var  << "  at Address: " << (void*)&local_var  << "\n";
        std::cout << "----------------------------------------------------------------------\n";
        
        std::cout << "\n[CONCLUSION]\n";
        std::cout << "1. Are values shared? NO. Child modifications (+50) do NOT affect Parent.\n";
        std::cout << "2. Are Virtual Addresses identical in Parent and Child? YES.\n";
        std::cout << "   - Because fork() creates an exact copy of the Virtual Address Space.\n";
        std::cout << "   - Copy-On-Write (COW) ensures physical memory pages are separated when written to.\n";
        std::cout << "======================================================================\n";
    }

    return 0;
}
