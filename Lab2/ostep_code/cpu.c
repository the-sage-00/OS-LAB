#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string> [loops]\n", argv[0]);
        exit(1);
    }
    
    char *str = argv[1];
    int max_loops = (argc >= 3) ? atoi(argv[2]) : 5; // Default 5 iterations instead of infinite loop

    printf("Executing CPU Virtualization Demo for %d iterations...\n", max_loops);
    for (int i = 1; i <= max_loops; i++) {
        Spin(1);
        printf("[%d/%d] %s (PID: %d)\n", i, max_loops, str, getpid());
    }

    printf("Execution complete.\n");
    return 0;
}
