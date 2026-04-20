#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, m;

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter number of resources: ";
    cin >> m;

    vector<vector<int>> alloc(n, vector<int>(m)); // allocated process
    vector<vector<int>> max(n, vector<int>(m));   // max processes may be used by a process
    vector<vector<int>> need(n, vector<int>(m));  // the need matrix like which process will require by a process
    vector<int> avail(m);                         // the total availaible resources

    // Resource labels
    vector<char> res(m);
    for (int i = 0; i < m; i++)
        res[i] = 'A' + i;

    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> alloc[i][j];

    cout << "\nEnter Max Matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> max[i][j];

    cout << "\nEnter Available Resources:\n";
    for (int j = 0; j < m; j++)
        cin >> avail[j];

    // Need calculation
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    vector<bool> finish(n, false);
    vector<int> safeSeq;

    cout << "\n===== INITIAL TABLE =====\n";

    // Header
    cout << "Process  Allocation   Max   Need\n";
    cout << "         ";
    for (int j = 0; j < m; j++)
        cout << res[j] << " ";
    cout << "      ";
    for (int j = 0; j < m; j++)
        cout << res[j] << " ";
    cout << "      ";
    for (int j = 0; j < m; j++)
        cout << res[j] << " ";
    cout << endl;

    // Print table
    for (int i = 0; i < n; i++)
    {
        cout << "P" << i + 1 << "       ";
        for (int j = 0; j < m; j++)
            cout << alloc[i][j] << " ";
        cout << "      ";
        for (int j = 0; j < m; j++)
            cout << max[i][j] << " ";
        cout << "      ";
        for (int j = 0; j < m; j++)
            cout << need[i][j] << " ";
        cout << endl;
    }

    cout << "Available: ";
    for (int j = 0; j < m; j++)
        cout << avail[j] << " ";
    cout << "\n\n";

    // Banker's Algorithm
    //we will check for every possible process if the need satisfies we will allocate that process and we will increase the avail with whatever there in alloc
    while (safeSeq.size() < n)
    {
        bool found = false;

        for (int i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                bool possible = true;

                for (int j = 0; j < m; j++)
                {
                    if (need[i][j] > avail[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if (possible)
                {
                    cout << ">>> Executing P" << i + 1 << "\n";

                    for (int j = 0; j < m; j++)
                        avail[j] += alloc[i][j];

                    finish[i] = true;
                    safeSeq.push_back(i);

                    // Print updated table
                    cout << "\n===== AFTER P" << i + 1 << " =====\n";
                    cout << "Process  Allocation   Max   Need\n";

                    for (int k = 0; k < n; k++)
                    {
                        cout << "P" << k + 1 << "       ";
                        for (int j = 0; j < m; j++)
                            cout << alloc[k][j] << " ";
                        cout << "      ";
                        for (int j = 0; j < m; j++)
                            cout << max[k][j] << " ";
                        cout << "      ";
                        for (int j = 0; j < m; j++)
                            cout << need[k][j] << " ";
                        cout << (finish[k] ? "   (Done)" : "") << endl;
                    }

                    cout << "Available: ";
                    for (int j = 0; j < m; j++)
                        cout << avail[j] << " ";
                    cout << "\n\n";

                    found = true;
                }
            }
        }

        if (!found)
        {
            cout << "Deadlock possible\n";
            return 0;
        }
    }

    // Safe sequence
    cout << "SAFE SEQUENCE: ";
    for (int i : safeSeq)
        cout << "P" << i + 1 << " ";
    cout << "\n";

    // Gantt Chart
    cout << "\nGANTT CHART:\n| ";
    for (int i : safeSeq)
        cout << "P" << i + 1 << " | ";
    cout << endl;

    return 0;
}