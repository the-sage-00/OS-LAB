#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

int *buffer;
int n, in = 0, out = 0;
sem_t empty_slots, full_slots;

void producer() {
    for (int x = 1; x <= 10; x++) {
        sem_wait(&empty_slots);
        buffer[in] = x;
        in = (in + 1) % n;
        cout << "Item " + to_string(x) + " produced\n" << flush;
        sem_post(&full_slots);
        usleep(1000);
    }
}

void consumer() {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full_slots);
        int x = buffer[out];
        out = (out + 1) % n;
        cout << "Item " + to_string(x) + " consumed\n" << flush;
        sem_post(&empty_slots);
        usleep(5000);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./p1 <buffer size>\n";
        return 1;
    }
    n = atoi(argv[1]);
    buffer = new int[n];
    sem_init(&empty_slots, 0, n);
    sem_init(&full_slots, 0, 0);

    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
