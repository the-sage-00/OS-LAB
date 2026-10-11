#include <iostream>
using namespace std;

const int P = 5, R = 4;

bool isSafe(int avail[], int alloc[][R], int need[][R], int seq[]) {
    int work[R];
    bool done[P] = {false};
    int count = 0;
    for (int j = 0; j < R; j++)
        work[j] = avail[j];

    while (count < P) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (done[i]) continue;
            bool ok = true;
            for (int j = 0; j < R; j++)
                if (need[i][j] > work[j]) ok = false;
            if (ok) {
                for (int j = 0; j < R; j++)
                    work[j] += alloc[i][j];
                done[i] = true;
                seq[count++] = i;
                found = true;
            }
        }
        if (!found) return false;
    }
    return true;
}

void banker(int avail[], int alloc[][R], int maxm[][R], int p, int request[]) {
    int need[P][R], seq[P];
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = maxm[i][j] - alloc[i][j];

    cout << "Process   Allocation    Max       Need\n";
    for (int i = 0; i < P; i++) {
        cout << "P" << i << "        ";
        for (int j = 0; j < R; j++) cout << alloc[i][j] << " ";
        cout << "     ";
        for (int j = 0; j < R; j++) cout << maxm[i][j] << " ";
        cout << "    ";
        for (int j = 0; j < R; j++) cout << need[i][j] << " ";
        cout << "\n";
    }
    cout << "Available: ";
    for (int j = 0; j < R; j++) cout << avail[j] << " ";
    cout << "\n";

    cout << "\nP" << p << " requests: ";
    for (int j = 0; j < R; j++) cout << request[j] << " ";
    cout << "\n";

    for (int j = 0; j < R; j++) {
        if (request[j] > need[p][j]) {
            cout << "Error: process has exceeded its maximum claim\n";
            return;
        }
    }
    for (int j = 0; j < R; j++) {
        if (request[j] > avail[j]) {
            cout << "Resources not available, P" << p << " must wait\n";
            return;
        }
    }

    for (int j = 0; j < R; j++) {
        avail[j] -= request[j];
        alloc[p][j] += request[j];
        need[p][j] -= request[j];
    }

    if (isSafe(avail, alloc, need, seq)) {
        cout << "Request can be granted. Safe sequence: ";
        for (int i = 0; i < P; i++) {
            cout << "P" << seq[i];
            if (i < P - 1) cout << " -> ";
        }
        cout << "\n";
    } else {
        cout << "Request cannot be granted. No safe sequence exists, system would be unsafe\n";
        for (int j = 0; j < R; j++) {
            avail[j] += request[j];
            alloc[p][j] -= request[j];
            need[p][j] += request[j];
        }
    }
}

int main() {
    int maxm[P][R] = {{7, 5, 3, 2}, {3, 2, 2, 2}, {9, 0, 2, 2}, {2, 2, 2, 2}, {4, 3, 3, 2}};

    int alloc1[P][R] = {{0, 1, 0, 0}, {2, 0, 0, 1}, {3, 0, 2, 0}, {2, 1, 1, 1}, {0, 0, 2, 0}};
    int avail1[R] = {3, 3, 2, 1};
    int req1[R] = {1, 0, 2, 0};
    cout << "CASE 1: Safe sequence exists\n";
    banker(avail1, alloc1, maxm, 1, req1);

    int alloc2[P][R] = {{0, 1, 0, 0}, {2, 0, 0, 1}, {3, 0, 2, 0}, {2, 1, 1, 1}, {0, 0, 2, 0}};
    int avail2[R] = {3, 3, 2, 1};
    int req2[R] = {0, 0, 2, 0};
    cout << "\nCASE 2: Safe sequence does not exist\n";
    banker(avail2, alloc2, maxm, 0, req2);

    return 0;
}
