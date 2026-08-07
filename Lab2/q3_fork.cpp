/*
 * OS Lab Assignment 2 — Question 3: Child Process Creation using fork() [C++]
 * Demonstrates fork() system call, return values, PID inspection,
 * and execution path branching between Parent and Child using C++ (g++).
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    std::cout << "=== Question 3: Fork System Call Demo (C++) ===" << std::endl;
    std::cout << "Parent Process Started (PID: " << getpid() << ")\n" << std::endl;

    // Create a new child process
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        std::cerr << "fork failed!" << std::endl;
        return EXIT_FAILURE;
    } 
    else if (pid == 0) {
        // Child Process Branch
        std::cout << "[CHILD PROCESS]" << std::endl;
        std::cout << "  My PID          : " << getpid() << std::endl;
        std::cout << "  My Parent PID   : " << getppid() << std::endl;
        std::cout << "  fork() return   : " << pid << std::endl;
        std::cout << "Child process exiting gracefully...\n" << std::endl;
        exit(0);
    } 
    else {
        // Parent Process Branch
        std::cout << "[PARENT PROCESS]" << std::endl;
        std::cout << "  My PID          : " << getpid() << std::endl;
        std::cout << "  Created Child   : " << pid << std::endl;
        std::cout << "  fork() return   : " << pid << std::endl;
        
        // Wait for child to complete
        int status;
        wait(&status);
        std::cout << "Parent confirmed child (PID: " << pid << ") exited with status: " << WEXITSTATUS(status) << std::endl;
    }

    std::cout << "Main program ended cleanly." << std::endl;
    return 0;
}
