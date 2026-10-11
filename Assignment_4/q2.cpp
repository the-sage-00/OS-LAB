#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct Process
{
    int id;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int completion;
    bool done;
};

int main()
{

    int n;

    cout << "========================================\n";
    cout << "       CPU SCHEDULING - PART B\n";
    cout << "========================================\n";

    cout << "\nEnter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    cout << "\nEnter process details\n";
    cout << "-------------------------\n";

    for (int i = 0; i < n; i++)
    {

        p[i].id = i + 1;

        cout << "\nProcess P" << i + 1 << "\n";

        cout << "Arrival Time: ";
        cin >> p[i].arrival;

        cout << "CPU Burst: ";
        cin >> p[i].burst;

        p[i].remaining = p[i].burst;
        p[i].waiting = 0;
        p[i].completion = 0;
        p[i].done = false;
    }

    int choice;

    cout << "\n========================================\n";
    cout << "Select Scheduling Algorithm\n";
    cout << "========================================\n";
    cout << "1. FCFS\n";
    cout << "2. SJF Non-Preemptive\n";
    cout << "3. SJF Preemptive\n";
    cout << "Enter choice: ";
    cin >> choice;

    vector<int> sequence;

    /* ---------------- FCFS ---------------- */

    if (choice == 1)
    {

        int time = 0;
        int completed = 0;

        while (completed < n)
        {

            int index = -1;

            // Find earliest arrived process
            for (int i = 0; i < n; i++)
            {

                if (!p[i].done && p[i].arrival <= time)
                {

                    if (index == -1 ||
                        p[i].arrival < p[index].arrival)
                    {
                        index = i;
                    }
                }
            }

            // CPU is idle
            if (index == -1)
            {
                time++;
                continue;
            }

            p[index].waiting = time - p[index].arrival;

            time += p[index].burst;

            p[index].completion = time;
            p[index].done = true;

            sequence.push_back(p[index].id);
            completed++;
        }
    }

    /* ------------ SJF NON-PREEMPTIVE ------------ */

    else if (choice == 2)
    {

        int time = 0;
        int completed = 0;

        while (completed < n)
        {

            int index = -1;

            // Find shortest burst among arrived processes
            for (int i = 0; i < n; i++)
            {

                if (!p[i].done && p[i].arrival <= time)
                {

                    if (index == -1 ||
                        p[i].burst < p[index].burst)
                    {
                        index = i;
                    }
                }
            }

            if (index == -1)
            {
                time++;
                continue;
            }

            p[index].waiting = time - p[index].arrival;

            time += p[index].burst;

            p[index].completion = time;
            p[index].done = true;

            sequence.push_back(p[index].id);
            completed++;
        }
    }

    /* ------------ SJF PREEMPTIVE ------------ */

    else if (choice == 3)
    {

        int time = 0;
        int completed = 0;
        int last = -1;

        while (completed < n)
        {

            int index = -1;

            // Find process with smallest remaining time
            for (int i = 0; i < n; i++)
            {

                if (!p[i].done && p[i].arrival <= time)
                {

                    if (index == -1 ||
                        p[i].remaining < p[index].remaining)
                    {
                        index = i;
                    }
                }
            }

            if (index == -1)
            {
                time++;
                continue;
            }

            // Add process to sequence only when CPU changes
            if (last != index)
            {
                sequence.push_back(p[index].id);
                last = index;
            }

            p[index].remaining--;
            time++;

            // Process finished
            if (p[index].remaining == 0)
            {

                p[index].done = true;
                p[index].completion = time;

                // Waiting Time =
                // Completion - Arrival - Burst
                p[index].waiting =
                    p[index].completion - p[index].arrival - p[index].burst;

                completed++;
            }
        }
    }

    else
    {
        cout << "\nInvalid choice!\n";
        return 0;
    }

    /* ---------------- OUTPUT ---------------- */

    double average = 0;

    cout << "\n\n========================================\n";
    cout << "              RESULT\n";
    cout << "========================================\n";

    cout << left
         << setw(8) << "PID"
         << setw(12) << "Arrival"
         << setw(12) << "Burst"
         << setw(12) << "Waiting"
         << setw(12) << "Completion"
         << "\n";

    cout << "----------------------------------------\n";

    for (int i = 0; i < n; i++)
    {

        cout << left
             << setw(8) << ("P" + to_string(p[i].id))
             << setw(12) << p[i].arrival
             << setw(12) << p[i].burst
             << setw(12) << p[i].waiting
             << setw(12) << p[i].completion
             << "\n";

        average += p[i].waiting;
    }

    average /= n;

    cout << "\nAverage Waiting Time = "
         << fixed << setprecision(2)
         << average << "\n";

    cout << "\nExecution Sequence:\n";

    for (int i = 0; i < sequence.size(); i++)
    {

        cout << "P" << sequence[i];

        if (i != sequence.size() - 1)
            cout << " -> ";
    }

    cout << "\n";

    return 0;
}