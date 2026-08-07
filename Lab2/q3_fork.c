/*
 * OS Lab Assignment 2 — Question 3: Child Process Creation using fork()
 * Demonstrates fork() system call, return values, PID inspection,
 * and execution path branching between Parent and Child.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("=== Question 3: Fork System Call Demo ===\n");
    printf("Parent Process Started (PID: %d)\n\n", getpid());

    // Create a new child process
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Child Process Branch
        printf("[CHILD PROCESS]\n");
        printf("  My PID          : %d\n", getpid());
        printf("  My Parent PID   : %d\n", getppid());
        printf("  fork() return   : %d\n", pid);
        printf("Child process exiting gracefully...\n\n");
        exit(0);
    } 
    else {
        // Parent Process Branch
        printf("[PARENT PROCESS]\n");
        printf("  My PID          : %d\n", getpid());
        printf("  Created Child   : %d\n", pid);
        printf("  fork() return   : %d\n", pid);
        
        // Wait for child to complete
        int status;
        wait(&status);
        printf("Parent confirmed child (PID: %d) exited with status: %d\n", pid, WEXITSTATUS(status));
    }

    printf("Main program ended cleanly.\n");
    return 0;
}
