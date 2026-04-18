// shortest seek time first
#include <iostream>
#include <queue>
using namespace std;

void sstf(vector<int> req, int head, int disk_size)
{
    priority_queue<int> left;
    priority_queue<int, vector<int>, greater<int>> right;

    for (auto i : req)
    {
        if (i <= head)
            left.push(i);
        else
            right.push(i);
    }

    int total_seek_time = 0;

    while (!left.empty() && !right.empty())
    {
        int lt = abs(left.top() - head);
        int rt = abs(right.top() - head);
        if (lt <= rt)
        {
            total_seek_time += lt;
            head = left.top();
            left.pop();
        }
        else
        {
            total_seek_time += rt;
            head = right.top();
            right.pop();
        }
    }

    while (!left.empty())
    {
        int lt = abs(left.top() - head);
        total_seek_time += lt;
        head = left.top();
        left.pop();
    }

    while (!right.empty())
    {
        total_seek_time += abs(right.top() - head);
        head = right.top();
        right.pop();
    }

    cout << "\nTotal Seek Time: " << total_seek_time << endl;
}

void scan(vector<int> req, int head, int disk_size)
{
    int total_seek = 0;
    vector<int> left, right;

    for (int i = 0; i < req.size(); i++)
    {
        if (req[i] < head)
            left.push_back(req[i]);
        else
            right.push_back(req[i]);
    }

    sort(left.begin(), left.end(), greater<int>());
    sort(right.begin(), right.end());

    // will move towards right
    for (auto &&i : right)
    {
        total_seek += abs(head - i);
        head = i;
    }
    if (head != disk_size - 1)
    {
        total_seek += abs(head - (disk_size - 1));
        head = disk_size - 1;
    }
    for (auto &&i : left)
    {
        total_seek += abs(head - i);
        head = i;
    }

    cout<<"The total seek time is "<<total_seek<<endl;
}
int main()
{
    int n, head, disk_size;

    cout << "Enter number of requests: ";
    cin >> n;

    vector<int> req(n);
    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++)
    {
        cin >> req[i];
    }

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> disk_size;

    int head_move = 1;
    // 1-> move right
    // 0-> move left;

    // sstf(req, head, disk_size);
    scan(req,head,disk_size);
    return 0;
}