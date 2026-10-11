#include <stdio.h>
#include <pthread.h>

int data[10] = {24, 67, 13, 89, 45, 7, 92, 38, 56, 31};
int sum = 0, max, min;

void *find_sum(void *arg) {
    for (int i = 0; i < 10; i++)
        sum += data[i];
    return NULL;
}

void *find_max(void *arg) {
    max = data[0];
    for (int i = 1; i < 10; i++)
        if (data[i] > max) max = data[i];
    return NULL;
}

void *find_min(void *arg) {
    min = data[0];
    for (int i = 1; i < 10; i++)
        if (data[i] < min) min = data[i];
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, find_sum, NULL);
    pthread_create(&t2, NULL, find_max, NULL);
    pthread_create(&t3, NULL, find_min, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Sum = %d\n", sum);
    printf("Max = %d\n", max);
    printf("Min = %d\n", min);
    return 0;
}
