#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ACCOUNTS 3

int balance[ACCOUNTS] = {1000, 1000, 1000};
int deposited[ACCOUNTS], withdrawn[ACCOUNTS];
pthread_mutex_t lock[ACCOUNTS];

void *deposit(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 4; i++) {
        int acc = (id + i) % ACCOUNTS;
        int amt = 100 * (id + i);

        pthread_mutex_lock(&lock[acc]);
        balance[acc] += amt;
        deposited[acc] += amt;
        printf("Client %d deposited %d in account %d, balance = %d\n",
               id, amt, acc + 1, balance[acc]);
        pthread_mutex_unlock(&lock[acc]);
        usleep(2000);
    }
    return NULL;
}

void *withdraw(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 4; i++) {
        int acc = (id + 2 * i) % ACCOUNTS;
        int amt = 200 * (i + 1);

        pthread_mutex_lock(&lock[acc]);
        if (balance[acc] >= amt) {
            balance[acc] -= amt;
            withdrawn[acc] += amt;
            printf("Client %d withdrew %d from account %d, balance = %d\n",
                   id, amt, acc + 1, balance[acc]);
        } else {
            printf("Client %d failed to withdraw %d from account %d (low balance)\n",
                   id, amt, acc + 1);
        }
        pthread_mutex_unlock(&lock[acc]);
        usleep(2000);
    }
    return NULL;
}

int main() {
    pthread_t t1[2], t2[2];
    int id[2] = {1, 2};

    for (int i = 0; i < ACCOUNTS; i++)
        pthread_mutex_init(&lock[i], NULL);

    for (int i = 0; i < 2; i++) {
        pthread_create(&t1[i], NULL, deposit, &id[i]);
        pthread_create(&t2[i], NULL, withdraw, &id[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }

    printf("\n");
    for (int i = 0; i < ACCOUNTS; i++) {
        int expected = 1000 + deposited[i] - withdrawn[i];
        printf("Account %d: final = %d, expected = %d\n", i + 1, balance[i], expected);
    }
    return 0;
}
