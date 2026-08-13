#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("=== Demonstration of fork(), execv(), wait(), and exit() ===\n");
    printf("Parent PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed!\n");
        exit(1);
    } else if (pid == 0) {
        printf("[Child PID: %d] Process created using fork()\n", getpid());
        printf("[Child PID: %d] Executing /bin/ls using execv()...\n", getpid());

        char *args[] = {"/bin/ls", "-l", NULL};
        execv(args[0], args);

        // execv only returns if an error occurred
        printf("execv failed!\n");
        exit(1);
    } else {
        printf("[Parent PID: %d] Waiting for child (PID: %d) using wait()...\n", getpid(), pid);

        int status;
        pid_t child_pid = wait(&status);

        if (WIFEXITED(status)) {
            printf("[Parent PID: %d] Child process %d finished with exit code %d\n", getpid(), child_pid, WEXITSTATUS(status));
        }
    }

    return 0;
}
