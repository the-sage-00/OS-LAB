#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 6

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;

    // Dekker's variables
    int dekker_flag[2];
    int dekker_turn;

    // Lamport's Bakery variables
    int choosing[2];
    int number[2];

    // Peterson's variables
    int peterson_flag[2];
    int peterson_turn;
} SharedData;

// ============================================================
// 1. DEKKER'S ALGORITHM
// ============================================================
void dekker_entry(SharedData *s, int i) {
    int j = 1 - i;
    s->dekker_flag[i] = 1;
    while (s->dekker_flag[j]) {
        if (s->dekker_turn != i) {
            s->dekker_flag[i] = 0;
            while (s->dekker_turn != i) {
                usleep(100);
            }
            s->dekker_flag[i] = 1;
        }
    }
}

void dekker_exit(SharedData *s, int i) {
    int j = 1 - i;
    s->dekker_turn = j;
    s->dekker_flag[i] = 0;
}

// ============================================================
// 2. LAMPORT'S BAKERY ALGORITHM
// ============================================================
void lamport_entry(SharedData *s, int i) {
    int j = 1 - i;
    s->choosing[i] = 1;
    int max_num = (s->number[0] > s->number[1]) ? s->number[0] : s->number[1];
    s->number[i] = max_num + 1;
    s->choosing[i] = 0;

    while (s->choosing[j]) {
        usleep(100);
    }
    while (s->number[j] != 0 && (s->number[j] < s->number[i] || 
          (s->number[j] == s->number[i] && j < i))) {
        usleep(100);
    }
}

void lamport_exit(SharedData *s, int i) {
    s->number[i] = 0;
}

// ============================================================
// 3. PETERSON'S ALGORITHM
// ============================================================
void peterson_entry(SharedData *s, int i) {
    int j = 1 - i;
    s->peterson_flag[i] = 1;
    s->peterson_turn = j;
    while (s->peterson_flag[j] && s->peterson_turn == j) {
        usleep(100);
    }
}

void peterson_exit(SharedData *s, int i) {
    s->peterson_flag[i] = 0;
}

// ============================================================
// RUN SIMULATION FOR CHOSEN ALGORITHM
// ============================================================
void run_simulation(int choice) {
    SharedData *s = mmap(NULL, sizeof(SharedData), 
                         PROT_READ | PROT_WRITE, 
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (s == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Initialize shared state
    s->in = 0;
    s->out = 0;
    s->count = 0;
    s->dekker_flag[0] = 0; s->dekker_flag[1] = 0; s->dekker_turn = 0;
    s->choosing[0] = 0;    s->choosing[1] = 0;    s->number[0] = 0; s->number[1] = 0;
    s->peterson_flag[0] = 0; s->peterson_flag[1] = 0; s->peterson_turn = 0;

    const char *algo_name = "";
    if (choice == 1) algo_name = "Dekker's Algorithm";
    else if (choice == 2) algo_name = "Lamport's Bakery Algorithm";
    else if (choice == 3) algo_name = "Peterson's Algorithm";

    printf("\n====================================================\n");
    printf("  SIMULATING PRODUCER-CONSUMER USING: %s\n", algo_name);
    printf("  Buffer Size = %d | Total Items = %d\n", BUFFER_SIZE, TOTAL_ITEMS);
    printf("====================================================\n");
    fflush(stdout);

    pid_t pid = fork();

    if (pid == 0) {
        // ---------------- CONSUMER PROCESS (ID = 1) ----------------
        for (int i = 1; i <= TOTAL_ITEMS; i++) {
            while (s->count == 0) {
                usleep(500); // Wait if buffer empty
            }

            // Entry section
            if (choice == 1) dekker_entry(s, 1);
            else if (choice == 2) lamport_entry(s, 1);
            else if (choice == 3) peterson_entry(s, 1);

            // Critical Section
            int item = s->buffer[s->out];
            printf("   [Consumer] Consumed: %d from buffer[%d] | Buffer Items: %d\n", 
                   item, s->out, s->count - 1);
            s->out = (s->out + 1) % BUFFER_SIZE;
            s->count--;
            fflush(stdout);

            // Exit section
            if (choice == 1) dekker_exit(s, 1);
            else if (choice == 2) lamport_exit(s, 1);
            else if (choice == 3) peterson_exit(s, 1);

            usleep(25000); // Simulate consumption time
        }
        exit(0);
    } else {
        // ---------------- PRODUCER PROCESS (ID = 0) ----------------
        for (int i = 1; i <= TOTAL_ITEMS; i++) {
            while (s->count == BUFFER_SIZE) {
                usleep(500); // Wait if buffer full
            }

            // Entry section
            if (choice == 1) dekker_entry(s, 0);
            else if (choice == 2) lamport_entry(s, 0);
            else if (choice == 3) peterson_entry(s, 0);

            // Critical Section
            s->buffer[s->in] = i * 10;
            printf("[Producer] Produced: %d at buffer[%d]   | Buffer Items: %d\n", 
                   i * 10, s->in, s->count + 1);
            s->in = (s->in + 1) % BUFFER_SIZE;
            s->count++;
            fflush(stdout);

            // Exit section
            if (choice == 1) dekker_exit(s, 0);
            else if (choice == 2) lamport_exit(s, 0);
            else if (choice == 3) peterson_exit(s, 0);

            usleep(15000); // Simulate production time
        }

        wait(NULL); // Wait for consumer to finish
        printf("----------------------------------------------------\n");
        printf("Simulation finished successfully for %s!\n\n", algo_name);
        fflush(stdout);
        munmap(s, sizeof(SharedData));
    }
}

int main() {
    printf("====================================================\n");
    printf("     ASSIGNMENT 6 - QUESTION 1: SHARED MEMORY IPC\n");
    printf("====================================================\n");
    printf("Select Synchronization Algorithm:\n");
    printf("1. Dekker's Algorithm\n");
    printf("2. Lamport's Bakery Algorithm\n");
    printf("3. Peterson's Algorithm\n");
    printf("4. Run All Three Algorithms\n");
    printf("Enter choice: ");
    fflush(stdout);

    int choice;
    if (scanf("%d", &choice) != 1) {
        choice = 4;
    }

    if (choice >= 1 && choice <= 3) {
        run_simulation(choice);
    } else if (choice == 4) {
        run_simulation(1);
        run_simulation(2);
        run_simulation(3);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
