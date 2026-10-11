#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstdlib>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

const int ITEMS = 5;
int *prod_id, *item_no;
int n, in = 0, out = 0;
sem_t empty_slots, full_slots, mutex_lock;

void producer(int id) {
    for (int x = 1; x <= ITEMS; x++) {
        sem_wait(&empty_slots);
        sem_wait(&mutex_lock);
        prod_id[in] = id;
        item_no[in] = x;
        in = (in + 1) % n;
        cout << "Producer " + to_string(id) + ": item " + to_string(id) + "." +
                    to_string(x) + " produced\n" << flush;
        sem_post(&mutex_lock);
        sem_post(&full_slots);
        usleep(1000);
    }
}

void consumer(int id, int count) {
    for (int i = 0; i < count; i++) {
        sem_wait(&full_slots);
        sem_wait(&mutex_lock);
        int p = prod_id[out];
        int x = item_no[out];
        out = (out + 1) % n;
        cout << "    Consumer " + to_string(id) + ": item " + to_string(p) + "." +
                    to_string(x) + " consumed\n" << flush;
        sem_post(&mutex_lock);
        sem_post(&empty_slots);
        usleep(3000);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "Usage: ./p3 <buffer size> <producers> <consumers>\n";
        return 1;
    }
    n = atoi(argv[1]);
    int np = atoi(argv[2]);
    int nc = atoi(argv[3]);

    prod_id = new int[n];
    item_no = new int[n];
    sem_init(&empty_slots, 0, n);
    sem_init(&full_slots, 0, 0);
    sem_init(&mutex_lock, 0, 1);

    int total = np * ITEMS;
    vector<thread> threads;
    for (int i = 1; i <= np; i++)
        threads.push_back(thread(producer, i));
    for (int i = 1; i <= nc; i++) {
        int count = total / nc + (i <= total % nc ? 1 : 0);
        threads.push_back(thread(consumer, i, count));
    }
    for (auto &t : threads)
        t.join();
    return 0;
}
