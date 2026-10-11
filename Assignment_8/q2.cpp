#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Request {
    int pid, arrival, size, duration;
};

struct Block {
    int start, size;
    bool free;
    int end, used;
};

int memory;
vector<Request> requests;
const int MIN_LEFT = 3;

int findHole(vector<Block> &mem, int size, int type) {
    int pick = -1;
    for (int i = 0; i < mem.size(); i++) {
        if (!mem[i].free || mem[i].size < size) continue;
        if (pick == -1) pick = i;
        else if (type == 2 && mem[i].size < mem[pick].size) pick = i;
        else if (type == 3 && mem[i].size > mem[pick].size) pick = i;
        if (type == 1) break;
    }
    return pick;
}

bool allocate(vector<Block> &mem, Request r, int t, int type) {
    int i = findHole(mem, r.size, type);
    if (i == -1) return false;

    int left = mem[i].size - r.size;
    mem[i].free = false;
    mem[i].used = r.size;
    mem[i].end = t + r.duration;

    if (left >= MIN_LEFT) {
        Block rest = {mem[i].start + r.size, left, true, 0, 0};
        mem[i].size = r.size;
        mem.insert(mem.begin() + i + 1, rest);
    }
    return true;
}

void freeBlocks(vector<Block> &mem, int t) {
    for (int i = 0; i < mem.size(); i++)
        if (!mem[i].free && mem[i].end <= t)
            mem[i].free = true;

    for (int i = 0; i + 1 < mem.size(); ) {
        if (mem[i].free && mem[i + 1].free) {
            mem[i].size += mem[i + 1].size;
            mem.erase(mem.begin() + i + 1);
        } else {
            i++;
        }
    }
}

void report(vector<Block> &mem, int total, int done) {
    int freeMem = 0, biggest = 0, internal = 0;
    for (int i = 0; i < mem.size(); i++) {
        if (mem[i].free) {
            freeMem += mem[i].size;
            biggest = max(biggest, mem[i].size);
        } else {
            internal += mem[i].size - mem[i].used;
        }
    }
    double success = done * 100.0 / total;
    double external = (freeMem - biggest) * 100.0 / memory;
    double inter = internal * 100.0 / memory;
    cout << fixed << setprecision(2);
    cout << "After " << setw(2) << total << " requests: Success = " << setw(6) << success
         << "%  External = " << setw(5) << external
         << "%  Internal = " << setw(4) << inter << "%\n";
}

void simulate(int type, string name) {
    vector<Block> mem;
    mem.push_back({0, memory, true, 0, 0});
    vector<Request> waiting;
    int next = 0, arrived = 0, done = 0;

    cout << name << "\n";

    for (int t = 0; ; t++) {
        freeBlocks(mem, t);

        for (int i = 0; i < waiting.size(); ) {
            if (allocate(mem, waiting[i], t, type)) {
                done++;
                waiting.erase(waiting.begin() + i);
            } else {
                i++;
            }
        }

        while (next < requests.size() && requests[next].arrival <= t) {
            arrived++;
            if (requests[next].size <= memory)
                waiting.push_back(requests[next]);
            next++;

            for (int i = 0; i < waiting.size(); ) {
                if (allocate(mem, waiting[i], t, type)) {
                    done++;
                    waiting.erase(waiting.begin() + i);
                } else {
                    i++;
                }
            }
            if (arrived % 10 == 0)
                report(mem, arrived, done);
        }

        if (next == requests.size() && waiting.empty())
            break;
    }
    cout << "Total allocated: " << done << " of " << arrived << "\n\n";
}

int main() {
    ifstream file("alloc.dat");
    if (!file) {
        cout << "Cannot open alloc.dat\n";
        return 1;
    }

    file >> memory;
    Request r;
    while (file >> r.pid && r.pid != -1) {
        file >> r.arrival >> r.size >> r.duration;
        requests.push_back(r);
    }
    stable_sort(requests.begin(), requests.end(),
                [](Request a, Request b) { return a.arrival < b.arrival; });

    simulate(1, "FIRST FIT");
    simulate(2, "BEST FIT");
    simulate(3, "WORST FIT");
    return 0;
}
