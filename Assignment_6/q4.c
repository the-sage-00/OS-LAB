#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct shared {
    int VAR;
    sem_t sem;
};

int main() {
    struct shared *s = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    s->VAR = 10;
    sem_init(&s->sem, 1, 0);
    printf("VAR = %d\n", s->VAR);
    fflush(stdout);

    if (fork() == 0) {
        s->VAR = s->VAR - 2;
        printf("Child: VAR = %d\n", s->VAR);
        sem_post(&s->sem);
        exit(0);
    } else {
        sem_wait(&s->sem);
        s->VAR = s->VAR + 2;
        printf("Parent: VAR = %d\n", s->VAR);
        wait(NULL);
    }
    return 0;
}
