// Q3: Create child process using fork()
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main() {
    cout << "=== Fork Demo ===" << endl;
    cout << "Parent started, PID: " << getpid() << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cout << "fork failed!" << endl;
        return 1;
    }
    else if (pid == 0) {
        // child process
        cout << "\n[Child] My PID: " << getpid() << endl;
        cout << "[Child] Parent's PID: " << getppid() << endl;
        cout << "[Child] fork() returned: " << pid << endl;
        exit(0);
    }
    else {
        // parent process
        cout << "\n[Parent] My PID: " << getpid() << endl;
        cout << "[Parent] Child PID: " << pid << endl;

        int status;
        wait(&status);
        cout << "[Parent] Child exited with status: " << WEXITSTATUS(status) << endl;
    }

    cout << "\nDone." << endl;
    return 0;
}
