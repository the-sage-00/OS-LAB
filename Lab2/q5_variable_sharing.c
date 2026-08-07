/*
 * OS Lab Assignment 2 — Question 5: Variable Sharing & Virtual Memory Address Analysis
 * Tests whether Global, Static, and Local variables are shared between Parent and Child.
 * Prints virtual memory addresses and values before and after modification in child.
 */

#include <stdio.h>
#include <stdlib.h>
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

    printf("======================================================================\n");
    printf("        Question 5: Parent-Child Variable Sharing & Address Test       \n");
    printf("======================================================================\n");
    printf("[BEFORE FORK] Process PID: %d\n", getpid());
    printf("  global_var  = %d  at Address: %p\n", global_var, (void*)&global_var);
    printf("  static_var  = %d  at Address: %p\n", static_var, (void*)&static_var);
    printf("  local_var   = %d  at Address: %p\n", local_var, (void*)&local_var);
    printf("----------------------------------------------------------------------\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        printf("\n[CHILD PROCESS] PID: %d (Parent PID: %d)\n", getpid(), getppid());
        printf("  Initial values & addresses in Child:\n");
        printf("    global_var  = %d  at Address: %p\n", global_var, (void*)&global_var);
        printf("    static_var  = %d  at Address: %p\n", static_var, (void*)&static_var);
        printf("    local_var   = %d  at Address: %p\n", local_var, (void*)&local_var);

        printf("\n  --> Child modifying variables: +50 to each...\n");
        global_var += 50;
        static_var += 50;
        local_var  += 50;

        printf("  Values after modification in Child:\n");
        printf("    global_var  = %d  at Address: %p\n", global_var, (void*)&global_var);
        printf("    static_var  = %d  at Address: %p\n", static_var, (void*)&static_var);
        printf("    local_var   = %d  at Address: %p\n", local_var, (void*)&local_var);
        
        printf("Child process exiting...\n");
        exit(0);
    } 
    else {
        // --- PARENT PROCESS ---
        // Wait for child to complete its modifications first
        wait(NULL);

        printf("\n[PARENT PROCESS] PID: %d\n", getpid());
        printf("  Values & addresses in Parent after Child finished:\n");
        printf("    global_var  = %d  at Address: %p\n", global_var, (void*)&global_var);
        printf("    static_var  = %d  at Address: %p\n", static_var, (void*)&static_var);
        printf("    local_var   = %d  at Address: %p\n", local_var, (void*)&local_var);
        printf("----------------------------------------------------------------------\n");
        
        printf("\n[CONCLUSION]\n");
        printf("1. Are values shared? NO. Child modifications (+50) do NOT affect Parent.\n");
        printf("2. Are Virtual Addresses identical in Parent and Child? YES.\n");
        printf("   - Because fork() creates an exact copy of the Virtual Address Space.\n");
        printf("   - Copy-On-Write (COW) ensures physical memory pages are separated when written to.\n");
        printf("======================================================================\n");
    }

    return 0;
}
