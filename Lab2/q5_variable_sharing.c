#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global_var = 100;

int main() {
    static int static_var = 200;
    int local_var = 300;

    printf("=== Parent Process Before Fork (PID: %d) ===\n", getpid());
    printf("global_var addr: %p, val: %d\n", (void*)&global_var, global_var);
    printf("static_var addr: %p, val: %d\n", (void*)&static_var, static_var);
    printf("local_var  addr: %p, val: %d\n", (void*)&local_var, local_var);

    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed!\n");
        return 1;
    } else if (pid == 0) {
        printf("\n=== Child Process (PID: %d) ===\n", getpid());
        printf("global_var addr: %p, val: %d\n", (void*)&global_var, global_var);
        printf("static_var addr: %p, val: %d\n", (void*)&static_var, static_var);
        printf("local_var  addr: %p, val: %d\n", (void*)&local_var, local_var);

        printf("\n[Child] Modifying variable values (+50)...\n");
        global_var += 50;
        static_var += 50;
        local_var += 50;

        printf("[Child] After modification:\n");
        printf("global_var: %d, static_var: %d, local_var: %d\n", global_var, static_var, local_var);
        exit(0);
    } else {
        wait(NULL);
        printf("\n=== Parent Process After Child Exit (PID: %d) ===\n", getpid());
        printf("global_var addr: %p, val: %d\n", (void*)&global_var, global_var);
        printf("static_var addr: %p, val: %d\n", (void*)&static_var, static_var);
        printf("local_var  addr: %p, val: %d\n", (void*)&local_var, local_var);
    }

    return 0;
}
