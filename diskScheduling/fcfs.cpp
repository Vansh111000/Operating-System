#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void FCFS(vector<int> req, int head) {
    int total_seek = 0;

    cout << "\n--- FCFS Disk Scheduling ---\n";
    cout << "Order of execution: ";

    for (int i = 0; i < req.size(); i++) {
        cout << req[i] << " ";
        total_seek += abs(head - req[i]);
        head = req[i];
    }

    cout << "\nTotal Seek Time: " << total_seek << endl;
}
void CSCAN(vector<int> req, int head, int disk_size) {
    int total_seek = 0;
    vector<int> left, right;

    for (int i = 0; i < req.size(); i++) {
        if (req[i] < head)
            left.push_back(req[i]);
        else
            right.push_back(req[i]);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "\n--- CSCAN Disk Scheduling ---\n";
    cout << "Order of execution: ";
//as it totally up to us to make the head to any direction.
    // Move right side
    for (int i = 0; i < right.size(); i++) {
        cout << right[i] << " ";
        total_seek += abs(head - right[i]);
        head = right[i];
    }

    // Move to end
    if (head != disk_size - 1) {
        total_seek += abs(head - (disk_size - 1));
        head = disk_size - 1;
    }

    // Jump to start
    total_seek += (disk_size - 1);
    head = 0;

    // Move left side
    for (int i = 0; i < left.size(); i++) {
        cout << left[i] << " ";
        total_seek += abs(head - left[i]);
        head = left[i];
    }

    cout << "\nTotal Seek Time: " << total_seek << endl;
}

int main() {
    int n, head, disk_size;

    cout << "Enter number of requests: ";
    cin >> n;

    vector<int> req(n);
    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++) {
        cin >> req[i];
    }

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> disk_size;

    FCFS(req, head);
    CSCAN(req, head, disk_size);
    return 0;
}