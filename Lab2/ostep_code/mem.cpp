// OSTEP memory virtualization demo (finite loop version)
#include <bits/stdc++.h>
#include <unistd.h>
#include "common.h"
using namespace std;

int main(int argc, char *argv[]) {
    int n = (argc >= 2) ? atoi(argv[1]) : 5;

    int *p = new int(0);
    cout << "(PID:" << getpid() << ") p is at address: " << p << endl;

    for (int i = 1; i <= n; i++) {
        Spin(1);
        (*p)++;
        cout << "[" << i << "/" << n << "] (PID:" << getpid() << ") *p = " << *p << endl;
    }

    delete p;
    cout << "Done." << endl;
    return 0;
}
