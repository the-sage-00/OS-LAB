// OSTEP cpu virtualization demo (finite loop version)
#include <bits/stdc++.h>
#include <unistd.h>
#include "common.h"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./cpu <label> [loops]" << endl;
        return 1;
    }

    string label = argv[1];
    int n = (argc >= 3) ? atoi(argv[2]) : 5;

    for (int i = 1; i <= n; i++) {
        Spin(1);
        cout << "[" << i << "/" << n << "] " << label << " (PID: " << getpid() << ")" << endl;
    }
    cout << "Done." << endl;
    return 0;
}
