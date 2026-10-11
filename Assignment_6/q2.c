#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 3
#define ITEMS 2

int buffer[SIZE];
int in = 0, out = 0, count = 0;
int consumed = 0, total = 0;

volatile int level[10];
volatile int victim[10];
int n, np;

int someone_ahead(int id, int L) {
    for (int k = 0; k < n; k++)
        if (k != id && level[k] >= L) return 1;
    return 0;
}

// peterson's algorithm for n threads (for n = 2 it is the normal one)
void enter(int id) {
    for (int L = 1; L < n; L++) {
        level[id] = L;
        victim[L] = id;
        __sync_synchronize();
        while (victim[L] == id && someone_ahead(id, L))
            usleep(50);
    }
}

void leave(int id) {
    level[id] = 0;
}

void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 1; i <= ITEMS; i++) {
        int done = 0;
        while (!done) {
            enter(id);
            if (count < SIZE) {
                buffer[in] = (id + 1) * 100 + i;
                printf("Producer %d produced %d\n", id + 1, buffer[in]);
                in = (in + 1) % SIZE;
                count++;
                done = 1;
            }
            leave(id);
            usleep(1000);
        }
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        enter(id);
        if (consumed == total) {
            leave(id);
            break;
        }
        if (count > 0) {
            printf("    Consumer %d consumed %d\n", id - np + 1, buffer[out]);
            out = (out + 1) % SIZE;
            count--;
            consumed++;
        }
        leave(id);
        usleep(6000);
    }
    return NULL;
}

void run(int p, int c) {
    pthread_t t[10];
    int id[10];
    n = p + c;
    np = p;
    total = p * ITEMS;
    in = out = count = consumed = 0;
    for (int i = 0; i < 10; i++) level[i] = victim[i] = 0;

    for (int i = 0; i < n; i++) {
        id[i] = i;
        if (i < p) pthread_create(&t[i], NULL, producer, &id[i]);
        else       pthread_create(&t[i], NULL, consumer, &id[i]);
    }
    for (int i = 0; i < n; i++)
        pthread_join(t[i], NULL);
}

int main() {
    printf("1) Single Producer - Single Consumer\n");
    run(1, 1);
    printf("\n2) Single Producer - Multiple Consumers\n");
    run(1, 3);
    printf("\n3) Multiple Producers - Single Consumer\n");
    run(3, 1);
    printf("\n4) Multiple Producers - Multiple Consumers\n");
    run(3, 3);
    return 0;
}
