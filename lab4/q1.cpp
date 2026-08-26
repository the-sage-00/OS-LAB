#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

struct Process
{
    int id, priority, arrival;
    vector<int> burst;
    int index, remaining;
    string history;
};

vector<Process> original;
int n, quantum = 1;

// ============================================================
// READ proc.dat AND PRINT INPUT CLEARLY
// ============================================================

void readFile()
{
    ifstream file("proc.dat");
    if (!file) { cout << "Error: proc.dat not found!\n"; return; }

    file >> n;
    file.ignore();
    original.resize(n);

    cout << "--------------------------------------------\n";
    cout << " INPUT (read from proc.dat)\n";
    cout << "--------------------------------------------\n";
    cout << left
         << setw(6)  << "PID"
         << setw(10) << "Priority"
         << setw(10) << "Arrival"
         << "Bursts (CPU then IO alternating)\n";
    cout << "--------------------------------------------\n";

    for (int i = 0; i < n; i++)
    {
        string line;
        getline(file, line);
        stringstream ss(line);
        string x;
        vector<int> v;

        while (getline(ss, x, ','))
        {
            int val = stoi(x);
            if (val == -1) break;
            v.push_back(val);
        }

        original[i].id       = v[0];
        original[i].priority = v[1];
        original[i].arrival  = v[2];

        for (int j = 3; j < (int)v.size(); j++)
            original[i].burst.push_back(v[j]);

        original[i].index     = 0;
        original[i].remaining = original[i].burst[0];
        original[i].history   = "";

        cout << left
             << setw(6)  << ("P" + to_string(v[0]))
             << setw(10) << v[1]
             << setw(10) << v[2];

        bool isCPU = true;
        for (int j = 3; j < (int)v.size(); j++)
        {
            cout << (isCPU ? "CPU=" : " IO=") << v[j] << " ";
            isCPU = !isCPU;
        }
        cout << "\n";
    }

    if (!(file >> quantum)) quantum = 1;
    cout << "\nQuantum = " << quantum << "\n";
    cout << "--------------------------------------------\n";
}

// ============================================================
// HELPERS
// ============================================================

bool finished(Process &p) { return p.index >= (int)p.burst.size(); }

bool ready(Process &p, int t)
{
    return !finished(p) && t >= p.arrival && p.index % 2 == 0;
}

void runIO(vector<Process> &p)
{
    for (int i = 0; i < n; i++)
        if (!finished(p[i]) && p[i].index % 2 == 1)
        {
            p[i].remaining--;
            if (p[i].remaining == 0)
            {
                p[i].index++;
                if (!finished(p[i]))
                    p[i].remaining = p[i].burst[p[i].index];
            }
        }
}

// Add one char to each process history for this time unit
void addHistory(vector<Process> &p, int cur, int t)
{
    for (int i = 0; i < n; i++)
    {
        if      (i == cur)                                    p[i].history += 'C';
        else if (!finished(p[i]) && p[i].index % 2 == 1)     p[i].history += 'R';
        else if (ready(p[i], t))                              p[i].history += 'W';
        else if (p[i].arrival <= t && !finished(p[i]))       p[i].history += 'B';
        else if (finished(p[i]))                              p[i].history += ' ';
        else                                                  p[i].history += '.';
    }
}

// ============================================================
// PRINT RESULT — clean table with time ruler
// ============================================================

void printResult(string name, vector<Process> &p)
{
    // Trim to actual end time
    int len = 0;
    for (int i = 0; i < n; i++)
    {
        int end = (int)p[i].history.size();
        while (end > 0 && (p[i].history[end-1]==' ' || p[i].history[end-1]=='.'))
            end--;
        if (end > len) len = end;
    }

    cout << "\n============================================\n";
    cout << "  " << name << "\n";
    cout << "============================================\n";
    cout << "C=CPU  W=Waiting  R=IO  B=Blocked  .=Not arrived\n\n";

    // Time index ruler
    cout << left << setw(16) << "Process";
    for (int t = 0; t < len; t++) cout << (t % 10);
    cout << "\n" << string(16 + len, '-') << "\n";

    // One row per process
    for (int i = 0; i < n; i++)
    {
        string label = "P" + to_string(p[i].id)
                     + "(arr=" + to_string(p[i].arrival)
                     + " pri=" + to_string(p[i].priority) + ")";
        cout << left << setw(16) << label;
        cout << p[i].history.substr(0, len) << "\n";
    }

    // Summary table
    cout << "\n" << left
         << setw(8)  << "Process"
         << setw(12) << "CPU units"
         << setw(10) << "IO units"
         << setw(12) << "Wait units" << "\n";
    cout << string(42, '-') << "\n";

    for (int i = 0; i < n; i++)
    {
        int cpu=0, io=0, wait=0;
        for (char c : p[i].history)
        {
            if      (c == 'C') cpu++;
            else if (c == 'R') io++;
            else if (c == 'W') wait++;
        }
        cout << left
             << setw(8)  << ("P" + to_string(p[i].id))
             << setw(12) << cpu
             << setw(10) << io
             << setw(12) << wait << "\n";
    }
}

// ============================================================
// FCFS
// ============================================================

void FCFS()
{
    vector<Process> p = original;
    int time=0, completed=0, current=-1;
    int limit = 500; // safety cap

    while (completed < n && time < limit)
    {
        if (current == -1)
        {
            int best = -1;
            for (int i = 0; i < n; i++)
                if (ready(p[i], time) && (best==-1 || p[i].arrival < p[best].arrival))
                    best = i;
            current = best;
        }

        addHistory(p, current, time);

        if (current != -1)
        {
            p[current].remaining--;
            if (p[current].remaining == 0)
            {
                p[current].index++;
                if (finished(p[current])) completed++;
                else p[current].remaining = p[current].burst[p[current].index];
                current = -1;
            }
        }
        runIO(p);
        time++;
    }
    printResult("FCFS", p);
}

// ============================================================
// SJF PREEMPTIVE
// ============================================================

void SJF()
{
    vector<Process> p = original;
    int time=0, completed=0;
    int limit = 500;

    while (completed < n && time < limit)
    {
        int cur = -1;
        for (int i = 0; i < n; i++)
            if (ready(p[i], time) && (cur==-1 || p[i].remaining < p[cur].remaining))
                cur = i;

        addHistory(p, cur, time);

        if (cur != -1)
        {
            p[cur].remaining--;
            if (p[cur].remaining == 0)
            {
                p[cur].index++;
                if (finished(p[cur])) completed++;
                else p[cur].remaining = p[cur].burst[p[cur].index];
            }
        }
        runIO(p);
        time++;
    }
    printResult("SJF (Preemptive)", p);
}

// ============================================================
// PRIORITY PREEMPTIVE
// ============================================================

void Priority()
{
    vector<Process> p = original;
    int time=0, completed=0;
    int limit = 500;

    while (completed < n && time < limit)
    {
        int cur = -1;
        for (int i = 0; i < n; i++)
            if (ready(p[i], time) && (cur==-1 || p[i].priority < p[cur].priority))
                cur = i;

        addHistory(p, cur, time);

        if (cur != -1)
        {
            p[cur].remaining--;
            if (p[cur].remaining == 0)
            {
                p[cur].index++;
                if (finished(p[cur])) completed++;
                else p[cur].remaining = p[cur].burst[p[cur].index];
            }
        }
        runIO(p);
        time++;
    }
    printResult("Priority (Preemptive)", p);
}

// ============================================================
// ROUND ROBIN
// ============================================================

void RoundRobin()
{
    vector<Process> p = original;
    int time=0, completed=0, current=-1, used=0;
    int limit = 500;

    while (completed < n && time < limit)
    {
        if (current == -1)
        {
            int best = -1;
            for (int i = 0; i < n; i++)
                if (ready(p[i], time) && (best==-1 || p[i].arrival < p[best].arrival))
                    best = i;
            current = best;
            used = 0;
        }

        addHistory(p, current, time);

        if (current != -1)
        {
            p[current].remaining--;
            used++;

            if (p[current].remaining == 0)
            {
                p[current].index++;
                if (finished(p[current])) completed++;
                else p[current].remaining = p[current].burst[p[current].index];
                current = -1; used = 0;
            }
            else if (used == quantum)
            {
                current = -1; used = 0;
            }
        }
        runIO(p);
        time++;
    }
    printResult("Round Robin (Quantum=" + to_string(quantum) + ")", p);
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    cout << "============================================\n";
    cout << "        CPU SCHEDULING SIMULATOR\n";
    cout << "============================================\n\n";

    readFile();

    FCFS();
    SJF();
    Priority();
    RoundRobin();

    return 0;
}