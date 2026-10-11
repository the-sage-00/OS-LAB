#include <iostream>
#include <string>
#include <cstdlib>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

struct Shared {
    int in, out;
    sem_t empty_slots, full_slots;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./p2 <buffer size>\n";
        return 1;
    }
    int n = atoi(argv[1]);

    Shared *s = (Shared *)mmap(NULL, sizeof(Shared), PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *buffer = (int *)mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    s->in = 0;
    s->out = 0;
    sem_init(&s->empty_slots, 1, n);
    sem_init(&s->full_slots, 1, 0);

    if (fork() == 0) {
        for (int x = 1; x <= 10; x++) {
            sem_wait(&s->empty_slots);
            buffer[s->in] = x;
            s->in = (s->in + 1) % n;
            cout << "Item " + to_string(x) + " produced\n" << flush;
            sem_post(&s->full_slots);
            usleep(1000);
        }
        exit(0);
    }

    if (fork() == 0) {
        for (int i = 0; i < 10; i++) {
            sem_wait(&s->full_slots);
            int x = buffer[s->out];
            s->out = (s->out + 1) % n;
            cout << "Item " + to_string(x) + " consumed\n" << flush;
            sem_post(&s->empty_slots);
            usleep(5000);
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    return 0;
}
