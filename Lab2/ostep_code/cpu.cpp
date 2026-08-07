#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <string> [loops]" << std::endl;
        return 1;
    }
    
    char *str = argv[1];
    int max_loops = (argc >= 3) ? std::atoi(argv[2]) : 5; // Default 5 iterations instead of infinite loop

    std::cout << "Executing CPU Virtualization Demo for " << max_loops << " iterations...\n";
    for (int i = 1; i <= max_loops; i++) {
        Spin(1);
        std::cout << "[" << i << "/" << max_loops << "] " << str << " (PID: " << getpid() << ")\n";
    }

    std::cout << "Execution complete.\n";
    return 0;
}
