/*
 * OS Lab Assignment 2 — Question 6: Complete System Call Program [C++]
 * Implements fork(), wait(), exit(), and execv() system calls in C++ (g++).
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    std::cout << "======================================================================\n";
    std::cout << "     Question 6: Demonstration of fork(), wait(), exit(), execv() [C++] \n";
    std::cout << "======================================================================\n";
    std::cout << "Parent Process (PID: " << getpid() << ") starting...\n" << std::endl;

    // 1. System Call: fork()
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork failed!" << std::endl;
        // 2. System Call: exit() on error
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Child Process
        std::cout << "[CHILD PROCESS] PID: " << getpid() << std::endl;
        std::cout << "[CHILD PROCESS] Preparing arguments for execv()..." << std::endl;

        // Arguments for execv (must be NULL-terminated array)
        char *args[] = { (char*)"/bin/ls", (char*)"-l", (char*)"-h", NULL };

        std::cout << "[CHILD PROCESS] Calling execv('/bin/ls', args)..." << std::endl;
        std::cout << "----------------------------------------------------------------------" << std::endl;
        
        // 3. System Call: execv()
        execv(args[0], args);

        // If execv returns, it means an error occurred
        std::cerr << "execv failed!" << std::endl;
        // 4. System Call: exit()
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent Process
        std::cout << "[PARENT PROCESS] PID: " << getpid() << " created Child PID: " << pid << std::endl;
        std::cout << "[PARENT PROCESS] Waiting for child process to complete..." << std::endl;

        int status;
        // 5. System Call: wait()
        pid_t wpid = wait(&status);

        std::cout << "\n----------------------------------------------------------------------" << std::endl;
        std::cout << "[PARENT PROCESS] Child PID " << wpid << " terminated." << std::endl;
        if (WIFEXITED(status)) {
            std::cout << "[PARENT PROCESS] Child exited normally with status: " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "[PARENT PROCESS] Child terminated abnormally." << std::endl;
        }
        std::cout << "======================================================================" << std::endl;
    }

    return 0;
}
