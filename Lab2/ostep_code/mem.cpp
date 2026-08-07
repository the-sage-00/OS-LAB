#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int max_loops = (argc >= 2) ? std::atoi(argv[1]) : 5; // Default 5 iterations instead of infinite loop

    int *p = (int*) malloc(sizeof(int));
    assert(p != NULL);
    std::cout << "(PID:" << getpid() << ") address pointed to by p: " << (void*)p << "\n";
    *p = 0;

    std::cout << "Executing Memory Virtualization Demo for " << max_loops << " iterations...\n";
    for (int i = 1; i <= max_loops; i++) {
        Spin(1);
        *p = *p + 1;
        std::cout << "[" << i << "/" << max_loops << "] (PID:" << getpid() << ") p: " << *p << " (Address: " << (void*)p << ")\n";
    }

    free(p);
    std::cout << "Execution complete.\n";
    return 0;
}
