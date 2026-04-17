#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct process
{
    int pid, arrivaltime, bt, completionTime, wt, tat, startTime;
};

struct cmp
{
    bool operator()(process a, process b)
    {
        return a.bt > b.bt;
    }
};

void displayQueue(priority_queue<process, vector<process>, cmp> rq)
{
    cout << "Ready queue: ";
    while (!rq.empty())
    {
        process tmp = rq.top();
        cout << "P" << tmp.pid << "(AT =" << tmp.arrivaltime << ") ";
        rq.pop();
    }
    cout << endl;
}

int main()
{
    int n;
    cout << "Enter number of processes : ";
    cin >> n;

    vector<process> p(n); // it will hold all the processes

    for (size_t i = 0; i < n; i++)
    {
        cout << "\nThe current process : " << i << endl;
        p[i].pid = i + 1;
        cout << "Enter the Process " << i + 1 << " Arrival time : ";
        cin >> p[i].arrivaltime;
        cout << "Enter the process " << i + 1 << " Burst time : ";
        cin >> p[i].bt;
    }

    sort(p.begin(), p.end(), [](process a, process b)
         { return a.arrivaltime < b.arrivaltime; }); // till here its all perfect

    // we can also display initial process here

    priority_queue<process, vector<process>, cmp> readyQueue;

    // logic for first come first serve
    int index = 0, currentTime = 0, ttat = 0, twt = 0;
    vector<process> exeorder;

    while (index < n || !readyQueue.empty())
    {
        while (index < n && p[index].arrivaltime <= currentTime)
        {
            readyQueue.push(p[index]);
            index++;
        }

        if (readyQueue.empty())
        {
            currentTime = p[index].arrivaltime;
            while (index < n && p[index].arrivaltime <= currentTime)
            {
                readyQueue.push(p[index]);
                index++;
            }
        }

        // now the all the processes are being placed in the queue
        priority_queue<process, vector<process>, cmp> qu = readyQueue;
        displayQueue(qu);

        process current = readyQueue.top();
        readyQueue.pop();

        current.startTime = currentTime;
        currentTime += current.bt;
        current.completionTime = currentTime;
        current.tat = current.completionTime - current.arrivaltime;
        current.wt = current.tat - current.bt;

        ttat += current.tat;
        twt += current.wt;

        exeorder.push_back(current);
        for (size_t i = 0; i < n; i++)
        {
            if (current.pid == p[i].pid)
            {
                p[i] = current;
                break;
            }
        }
    }

    cout << "\nGantt Chart\n";
    cout << "|";
    for (auto &&i : exeorder)
    {
        cout << " P" << i.pid << " |";
    }

    cout << endl;

    cout << "\n \nProcess\taT\tbT\tST\tCt\tTaT\tWt\n";
    for (auto &&i : p)
    {
        cout << "P" << i.pid << "\t"
             << i.arrivaltime << "\t"
             << i.bt << "\t"
             << i.startTime << "\t"
             << i.completionTime << "\t"
             << i.tat << "\t"
             << i.wt << endl;
    }

    cout << fixed << setprecision(6);
    cout << "Average Turnaround time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;

    return 0;
}