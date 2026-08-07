#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "common.h"

volatile int counter = 0;
int loops;

void *worker(void *arg) {
    for (int i = 0; i < loops; i++) {
        counter++;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <loops>\n";
        return 1;
    }
    loops = std::atoi(argv[1]);
    pthread_t p1, p2;

    std::cout << "Initial counter value: " << counter << "\n";

    pthread_create(&p1, NULL, worker, NULL);
    pthread_create(&p2, NULL, worker, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    std::cout << "Final counter value  : " << counter << " (Expected: " << loops * 2 << ")\n";
    return 0;
}
