#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int globalVar = 10;

int main() {
    static int staticVar = 20;
    int localVar = 30;

    cout << "Before fork:\n";
    cout << "Global: " << globalVar << " (addr: " << &globalVar << ")" << endl;
    cout << "Static: " << staticVar << " (addr: " << &staticVar << ")" << endl;
    cout << "Local:  " << localVar  << " (addr: " << &localVar  << ")" << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed\n";
        return 1;
    }

    if (pid == 0) {
        // Child modifies all variables
        globalVar = 100;
        staticVar = 200;
        localVar = 300;

        cout << "\nChild:\n";
        cout << "Global: " << globalVar << " (addr: " << &globalVar << ")" << endl;
        cout << "Static: " << staticVar << " (addr: " << &staticVar << ")" << endl;
        cout << "Local:  " << localVar  << " (addr: " << &localVar  << ")" << endl;
    }
    else {
        wait(NULL);

        cout << "\nParent:\n";
        cout << "Global: " << globalVar << " (addr: " << &globalVar << ")" << endl;
        cout << "Static: " << staticVar << " (addr: " << &staticVar << ")" << endl;
        cout << "Local:  " << localVar  << " (addr: " << &localVar  << ")" << endl;
    }

    return 0;
}
