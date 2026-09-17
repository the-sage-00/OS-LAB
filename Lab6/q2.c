#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define MAX_PROC 6

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;

    // Classic 2-Process Peterson's variables
    int flag[2];
    int turn;

    // Generalized Peterson's Filter Algorithm variables (for N processes)
    int level[MAX_PROC];
    int waiting[MAX_PROC - 1];

    int items_produced;
    int items_consumed;
    int total_to_produce;
} SharedData;

// ============================================================
// CLASSIC 2-PROCESS PETERSON'S ALGORITHM
// ============================================================
void peterson2_entry(SharedData *s, int i) {
    int j = 1 - i;
    s->flag[i] = 1;
    s->turn = j;
    while (s->flag[j] && s->turn == j) {
        usleep(50);
    }
}

void peterson2_exit(SharedData *s, int i) {
    s->flag[i] = 0;
}

// ============================================================
// GENERALIZED PETERSON'S FILTER ALGORITHM (FOR N PROCESSES)
// ============================================================
void petersonN_entry(SharedData *s, int i, int n) {
    for (int l = 0; l < n - 1; l++) {
        s->level[i] = l;
        s->waiting[l] = i;

        while (s->waiting[l] == i) {
            int exists = 0;
            for (int k = 0; k < n; k++) {
                if (k != i && s->level[k] >= l) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) break;
            usleep(50);
        }
    }
}

void petersonN_exit(SharedData *s, int i) {
    s->level[i] = -1;
}

// ============================================================
// PRODUCER PROCESS
// ============================================================
void producer_task(SharedData *s, int prod_id, int num_procs, int items_per_prod, int is_classic) {
    for (int i = 1; i <= items_per_prod; i++) {
        while (s->count == BUFFER_SIZE) {
            usleep(200); // Buffer is full
        }

        // Enter Critical Section
        if (is_classic) {
            peterson2_entry(s, 0);
        } else {
            petersonN_entry(s, prod_id, num_procs);
        }

        // Critical Section
        s->items_produced++;
        int val = s->items_produced * 10;
        s->buffer[s->in] = val;
        printf("[Producer %d] Produced: %d at buffer[%d] | Count = %d\n", 
               prod_id, val, s->in, s->count + 1);
        s->in = (s->in + 1) % BUFFER_SIZE;
        s->count++;
        fflush(stdout);

        // Exit Critical Section
        if (is_classic) {
            peterson2_exit(s, 0);
        } else {
            petersonN_exit(s, prod_id);
        }

        usleep(15000);
    }
    exit(0);
}

// ============================================================
// CONSUMER PROCESS
// ============================================================
void consumer_task(SharedData *s, int cons_id, int proc_idx, int num_procs, int is_classic) {
    while (1) {
        // Check termination condition
        if (s->items_consumed >= s->total_to_produce) {
            break;
        }

        while (s->count == 0) {
            if (s->items_consumed >= s->total_to_produce) return;
            usleep(200); // Buffer empty
        }

        // Enter Critical Section
        if (is_classic) {
            peterson2_entry(s, 1);
        } else {
            petersonN_entry(s, proc_idx, num_procs);
        }

        // Double check count inside critical section
        if (s->count > 0 && s->items_consumed < s->total_to_produce) {
            int item = s->buffer[s->out];
            s->items_consumed++;
            printf("   [Consumer %d] Consumed: %d from buffer[%d] | Count = %d\n", 
                   cons_id, item, s->out, s->count - 1);
            s->out = (s->out + 1) % BUFFER_SIZE;
            s->count--;
            fflush(stdout);
        }

        // Exit Critical Section
        if (is_classic) {
            peterson2_exit(s, 1);
        } else {
            petersonN_exit(s, proc_idx);
        }

        usleep(25000);
    }
    exit(0);
}

// ============================================================
// RUN CONFIGURATION
// ============================================================
void run_version(int version) {
    int num_producers = 1;
    int num_consumers = 1;
    const char *title = "";

    if (version == 1) {
        title = "Version (1): Single Producer - Single Consumer";
        num_producers = 1;
        num_consumers = 1;
    } else if (version == 2) {
        title = "Version (2): Single Producer - Multiple Consumers (2 Consumers)";
        num_producers = 1;
        num_consumers = 2;
    } else if (version == 3) {
        title = "Version (3): Multiple Producers (2 Producers) - Single Consumer";
        num_producers = 2;
        num_consumers = 1;
    } else if (version == 4) {
        title = "Version (4): Multiple Producers (2) - Multiple Consumers (2)";
        num_producers = 2;
        num_consumers = 2;
    }

    int total_procs = num_producers + num_consumers;
    int items_per_producer = 4;
    int total_items = num_producers * items_per_producer;

    printf("\n============================================================\n");
    printf("  %s\n", title);
    printf("  Producers: %d | Consumers: %d | Total Items: %d\n", 
           num_producers, num_consumers, total_items);
    printf("============================================================\n");
    fflush(stdout);

    SharedData *s = mmap(NULL, sizeof(SharedData), 
                         PROT_READ | PROT_WRITE, 
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    s->in = 0;
    s->out = 0;
    s->count = 0;
    s->items_produced = 0;
    s->items_consumed = 0;
    s->total_to_produce = total_items;
    s->flag[0] = 0; s->flag[1] = 0; s->turn = 0;

    for (int k = 0; k < MAX_PROC; k++) s->level[k] = -1;
    for (int k = 0; k < MAX_PROC - 1; k++) s->waiting[k] = -1;

    int is_classic = (total_procs == 2);

    // Fork Producers
    for (int p = 0; p < num_producers; p++) {
        pid_t pid = fork();
        if (pid == 0) {
            producer_task(s, p + 1, total_procs, items_per_producer, is_classic);
        }
    }

    // Fork Consumers
    for (int c = 0; c < num_consumers; c++) {
        pid_t pid = fork();
        if (pid == 0) {
            int proc_idx = num_producers + c;
            consumer_task(s, c + 1, proc_idx, total_procs, is_classic);
        }
    }

    // Wait for all children to finish
    for (int i = 0; i < total_procs; i++) {
        wait(NULL);
    }

    printf("------------------------------------------------------------\n");
    printf("Completed %s successfully!\n\n", title);
    fflush(stdout);
    munmap(s, sizeof(SharedData));
}

int main() {
    printf("============================================================\n");
    printf("   ASSIGNMENT 6 - QUESTION 2: PRODUCER-CONSUMER WITH PETERSON\n");
    printf("============================================================\n");
    printf("Select Version to Simulate:\n");
    printf("1. Single Producer - Single Consumer\n");
    printf("2. Single Producer - Multiple Consumers\n");
    printf("3. Multiple Producers - Single Consumer\n");
    printf("4. Multiple Producers - Multiple Consumers\n");
    printf("5. Run All 4 Versions\n");
    printf("Enter choice: ");
    fflush(stdout);

    int choice;
    if (scanf("%d", &choice) != 1) {
        choice = 5;
    }

    if (choice >= 1 && choice <= 4) {
        run_version(choice);
    } else if (choice == 5) {
        run_version(1);
        run_version(2);
        run_version(3);
        run_version(4);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
