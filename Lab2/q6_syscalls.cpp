#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main() {
    cout << "Parent process started." << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed." << endl;
        return 1;
    }

    if (pid == 0) {
        // Child process
        cout << "Child process started." << endl;
        cout << "Child PID: " << getpid() << endl;

        // Replace child program with ls
        execl("/bin/ls", "ls", NULL);

        // This executes only if exec fails
        cout << "exec failed." << endl;
        exit(1);
    }
    else {
        // Parent process
        cout << "Parent is waiting for child..." << endl;

        wait(NULL);

        cout << "Child has finished." << endl;
        cout << "Parent is exiting." << endl;
    }

    return 0;
}
