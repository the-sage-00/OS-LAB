// OSTEP concurrency demo - shows race condition with threads
#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

volatile int counter = 0;
int loops;

void *worker(void *arg) {
    for (int i = 0; i < loops; i++)
        counter++;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: ./threads <loops>" << endl;
        return 1;
    }
    loops = atoi(argv[1]);

    pthread_t t1, t2;
    cout << "Start: counter = " << counter << endl;

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    cout << "End: counter = " << counter << " (expected " << loops * 2 << ")" << endl;
    return 0;
}
