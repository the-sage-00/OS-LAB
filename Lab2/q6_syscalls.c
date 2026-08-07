/*
 * OS Lab Assignment 2 — Question 6: Complete System Call Program
 * Implements fork(), wait(), exit(), and execv() system calls.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("======================================================================\n");
    printf("     Question 6: Demonstration of fork(), wait(), exit(), execv()     \n");
    printf("======================================================================\n");
    printf("Parent Process (PID: %d) starting...\n\n", getpid());

    // 1. System Call: fork()
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        // 2. System Call: exit() on error
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Child Process
        printf("[CHILD PROCESS] PID: %d\n", getpid());
        printf("[CHILD PROCESS] Preparing arguments for execv()...\n");

        // Arguments for execv (must be NULL-terminated array)
        char *args[] = { "/bin/ls", "-l", "-h", NULL };

        printf("[CHILD PROCESS] Calling execv('/bin/ls', args)...\n");
        printf("----------------------------------------------------------------------\n");
        
        // 3. System Call: execv()
        execv(args[0], args);

        // If execv returns, it means an error occurred
        perror("execv failed");
        // 4. System Call: exit()
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent Process
        printf("[PARENT PROCESS] PID: %d created Child PID: %d\n", getpid(), pid);
        printf("[PARENT PROCESS] Waiting for child process to complete...\n");

        int status;
        // 5. System Call: wait()
        pid_t wpid = wait(&status);

        printf("\n----------------------------------------------------------------------\n");
        printf("[PARENT PROCESS] Child PID %d terminated.\n", wpid);
        if (WIFEXITED(status)) {
            printf("[PARENT PROCESS] Child exited normally with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("[PARENT PROCESS] Child terminated abnormally.\n");
        }
        printf("======================================================================\n");
    }

    return 0;
}
