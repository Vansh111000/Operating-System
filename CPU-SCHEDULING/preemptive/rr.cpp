// round robin
/* 
We are first adding, then tq calculation then check whether remaining time is 0. after then we pop the process from readyqueue then we add all the process happened in between that time quantum and after than add the process we poped off its still has some remaining time.
*/
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct process
{
    int pid, arrivaltime, bt, completionTime, wt, tat, startTime, remainingtime;
};

// struct cmp
/* {
    bool operator()(process a, process b)
    {
        return a.remainingtime > b.remainingtime;
    }
}; */

void displayQueue(queue<process> rq)
{
    cout << "Ready queue: ";
    while (!rq.empty())
    {
        process tmp = rq.front();
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

    int tq;
    cout << "Enter Time Quantum: ";
    cin >> tq;

    for (size_t i = 0; i < n; i++)
    {
        cout << "\nThe current process : " << i << endl;
        p[i].pid = i + 1;
        cout << "Enter the Process " << i + 1 << " Arrival time : ";
        cin >> p[i].arrivaltime;
        cout << "Enter the process " << i + 1 << " Burst time : ";
        cin >> p[i].bt;
        p[i].remainingtime = p[i].bt;
        p[i].startTime = -1;
    }

    sort(p.begin(), p.end(), [](process a, process b)
         { return a.arrivaltime < b.arrivaltime; }); // till here its all perfect

    // we can also display initial process here

    queue<process> readyQueue;

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
        queue<process> qu = readyQueue;
        displayQueue(qu);

        process current = readyQueue.front();

        if (current.startTime == -1)
            current.startTime = currentTime;

        int execTime = min(tq, current.remainingtime);
        current.remainingtime -= execTime;
        // cout << current.remainingtime << " \n";
        exeorder.push_back(current);
        readyQueue.pop();

        if (current.remainingtime == 0)
        {
            current.completionTime = currentTime+execTime;
            current.tat = current.completionTime - current.arrivaltime;
            current.wt = current.tat - current.bt;

            ttat += current.tat;
            twt += current.wt;
        }

        for (int t = 0; t < execTime; t++)
        {
            currentTime++;

            while (index < n && p[index].arrivaltime <= currentTime)
            {
                readyQueue.push(p[index]);
                index++;
            }
        }
        for (size_t i = 0; i < n; i++)
        {
            if (current.pid == p[i].pid)
            {
                p[i] = current;
                if (current.remainingtime != 0)
                    readyQueue.push(current);
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