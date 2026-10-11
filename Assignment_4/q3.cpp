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
    int priority;

    int waiting;
    int completion;

    bool done;
};

int main()
{

    int n;

    cout << "========================================\n";
    cout << "       CPU SCHEDULING - PART C\n";
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
    cout << "Select Algorithm\n";
    cout << "========================================\n";
    cout << "1. Priority Non-Preemptive\n";
    cout << "2. Priority Preemptive\n";
    cout << "3. Round Robin\n";
    cout << "Enter choice: ";
    cin >> choice;

    // Priority is needed only for priority scheduling
    if (choice == 1 || choice == 2)
    {

        cout << "\nEnter priority for each process\n";
        cout << "(Smaller number = Higher priority)\n";

        for (int i = 0; i < n; i++)
        {

            cout << "Priority of P" << i + 1 << ": ";
            cin >> p[i].priority;
        }
    }

    int quantum = 1;

    if (choice == 3)
    {

        cout << "\nEnter Quantum: ";
        cin >> quantum;
    }

    vector<int> sequence;

    /* =====================================
       PRIORITY NON-PREEMPTIVE
       ===================================== */

    if (choice == 1)
    {

        int time = 0;
        int completed = 0;

        while (completed < n)
        {

            int index = -1;

            for (int i = 0; i < n; i++)
            {

                if (!p[i].done && p[i].arrival <= time)
                {

                    if (index == -1 ||
                        p[i].priority < p[index].priority)
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

            p[index].waiting =
                time - p[index].arrival;

            time += p[index].burst;

            p[index].completion = time;
            p[index].done = true;

            sequence.push_back(p[index].id);

            completed++;
        }
    }

    /* =====================================
       PRIORITY PREEMPTIVE
       ===================================== */

    else if (choice == 2)
    {

        int time = 0;
        int completed = 0;
        int last = -1;

        while (completed < n)
        {

            int index = -1;

            for (int i = 0; i < n; i++)
            {

                if (!p[i].done && p[i].arrival <= time)
                {

                    if (index == -1 ||
                        p[i].priority < p[index].priority)
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

            // Add only when CPU changes process
            if (last != index)
            {
                sequence.push_back(p[index].id);
                last = index;
            }

            p[index].remaining--;
            time++;

            if (p[index].remaining == 0)
            {

                p[index].done = true;

                p[index].completion = time;

                p[index].waiting =
                    p[index].completion - p[index].arrival - p[index].burst;

                completed++;
            }
        }
    }

    /* =====================================
       ROUND ROBIN
       ===================================== */

    else if (choice == 3)
    {

        vector<int> queue;

        int front = 0;
        int time = 0;
        int completed = 0;

        // Add processes that arrive at time 0
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival == 0)
                queue.push_back(i);
        }

        while (completed < n)
        {

            // If queue is empty, move time forward
            if (front >= queue.size())
            {

                bool found = false;

                for (int i = 0; i < n; i++)
                {

                    if (!p[i].done &&
                        p[i].arrival > time)
                    {

                        time = p[i].arrival;
                        queue.push_back(i);
                        found = true;
                        break;
                    }
                }

                if (!found)
                    break;

                continue;
            }

            int index = queue[front];
            front++;

            // If process has not arrived yet
            if (p[index].arrival > time)
            {
                time = p[index].arrival;
            }

            sequence.push_back(p[index].id);

            int run = quantum;

            if (p[index].remaining < quantum)
                run = p[index].remaining;

            // Run process
            for (int j = 0; j < run; j++)
            {

                p[index].remaining--;
                time++;

                // Add newly arrived processes
                for (int k = 0; k < n; k++)
                {

                    if (!p[k].done &&
                        p[k].arrival == time &&
                        k != index)
                    {

                        queue.push_back(k);
                    }
                }

                if (p[index].remaining == 0)
                    break;
            }

            // Process finished
            if (p[index].remaining == 0)
            {

                p[index].done = true;
                p[index].completion = time;

                p[index].waiting =
                    p[index].completion - p[index].arrival - p[index].burst;

                completed++;
            }
            else
            {

                // Put process at end of queue
                queue.push_back(index);
            }
        }
    }

    else
    {

        cout << "\nInvalid choice!\n";
        return 0;
    }

    /* =====================================
       OUTPUT
       ===================================== */

    double average = 0;

    cout << "\n\n========================================\n";
    cout << "              RESULT\n";
    cout << "========================================\n";

    cout << left
         << setw(8) << "PID"
         << setw(12) << "Arrival"
         << setw(12) << "Burst";

    if (choice == 1 || choice == 2)
        cout << setw(12) << "Priority";

    cout << setw(12) << "Waiting"
         << setw(12) << "Completion"
         << "\n";

    cout << "----------------------------------------\n";

    for (int i = 0; i < n; i++)
    {

        cout << left
             << setw(8) << ("P" + to_string(p[i].id))
             << setw(12) << p[i].arrival
             << setw(12) << p[i].burst;

        if (choice == 1 || choice == 2)
            cout << setw(12) << p[i].priority;

        cout << setw(12) << p[i].waiting
             << setw(12) << p[i].completion
             << "\n";

        average += p[i].waiting;
    }

    average /= n;

    cout << "\nAverage Waiting Time = "
         << fixed << setprecision(2)
         << average << "\n";

    if (choice == 3)
        cout << "Quantum = " << quantum << "\n";

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