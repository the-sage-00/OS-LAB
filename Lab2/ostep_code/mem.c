#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int max_loops = (argc >= 2) ? atoi(argv[1]) : 5; // Default 5 iterations instead of infinite loop

    int *p = malloc(sizeof(int));
    assert(p != NULL);
    printf("(PID:%d) address pointed to by p: %p\n", getpid(), p);
    *p = 0;

    printf("Executing Memory Virtualization Demo for %d iterations...\n", max_loops);
    for (int i = 1; i <= max_loops; i++) {
        Spin(1);
        *p = *p + 1;
        printf("[%d/%d] (PID:%d) p: %d (Address: %p)\n", i, max_loops, getpid(), *p, p);
    }

    free(p);
    printf("Execution complete.\n");
    return 0;
}
