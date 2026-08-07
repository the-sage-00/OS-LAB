// Q5: Check if variables are shared between parent and child
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int global_var = 100; // global variable

int main() {
    static int static_var = 200; // static variable
    int local_var = 300;         // local variable

    cout << "=== Variable Sharing Test ===" << endl;
    cout << "Before fork (PID: " << getpid() << ")" << endl;
    cout << "  global = " << global_var << " addr: " << &global_var << endl;
    cout << "  static = " << static_var << " addr: " << &static_var << endl;
    cout << "  local  = " << local_var  << " addr: " << &local_var  << endl;
    cout << "---" << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cout << "fork failed!" << endl;
        return 1;
    }
    else if (pid == 0) {
        // child: show initial values then modify them
        cout << "\n[Child PID: " << getpid() << "]" << endl;
        cout << "  Before change:" << endl;
        cout << "    global = " << global_var << " addr: " << &global_var << endl;
        cout << "    static = " << static_var << " addr: " << &static_var << endl;
        cout << "    local  = " << local_var  << " addr: " << &local_var  << endl;

        // add 50 to each
        global_var += 50;
        static_var += 50;
        local_var  += 50;

        cout << "  After adding 50:" << endl;
        cout << "    global = " << global_var << endl;
        cout << "    static = " << static_var << endl;
        cout << "    local  = " << local_var  << endl;
        exit(0);
    }
    else {
        // parent waits, then checks its own values
        wait(NULL);

        cout << "\n[Parent PID: " << getpid() << "] after child finished:" << endl;
        cout << "    global = " << global_var << endl;
        cout << "    static = " << static_var << endl;
        cout << "    local  = " << local_var  << endl;

        cout << "\nResult: Child's changes did NOT affect parent." << endl;
        cout << "Addresses look same but physical memory is different (Copy-On-Write)." << endl;
    }

    return 0;
}
