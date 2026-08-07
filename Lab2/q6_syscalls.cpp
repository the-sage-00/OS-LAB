// Q6: Demo of fork(), wait(), exit(), execv()
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main() {
    cout << "=== System Calls Demo ===" << endl;
    cout << "Parent PID: " << getpid() << endl;

    pid_t pid = fork(); // create child

    if (pid < 0) {
        cout << "fork failed!" << endl;
        exit(1);
    }
    else if (pid == 0) {
        // child runs ls command using execv
        cout << "[Child] PID: " << getpid() << endl;
        cout << "[Child] Running execv to list files..." << endl;
        cout << "---" << endl;

        char *args[] = {(char*)"/bin/ls", (char*)"-l", (char*)"-h", NULL};
        execv(args[0], args);

        // this only runs if execv failed
        cout << "execv failed!" << endl;
        exit(1);
    }
    else {
        // parent waits for child
        cout << "[Parent] Waiting for child " << pid << "..." << endl;

        int status;
        pid_t w = wait(&status);

        cout << "---" << endl;
        cout << "[Parent] Child " << w << " finished";
        if (WIFEXITED(status))
            cout << " with exit code " << WEXITSTATUS(status);
        cout << endl;
    }

    return 0;
}
