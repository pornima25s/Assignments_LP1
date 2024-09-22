#include <iostream>
using namespace std;

void findWaitingTime(int processes[], int n, int bt[], int wt[])
{
    wt[0] = 0; // waiting time for first process is 0

    for (int i = 1; i < n; i++)
        wt[i] = bt[i - 1] + wt[i - 1];
}

void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[])
{
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];
}

void findavgTime(int processes[], int n, int bt[])
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    findWaitingTime(processes, n, bt, wt);
    findTurnAroundTime(processes, n, bt, wt, tat);

    cout << "--------------------------------------------------------------------" << endl;

    cout << "Processes " << " Burst time " << " Waiting time " << " Turn around time\n";

    // Calculate total waiting time and total turn around time
    for (int i = 0; i < n; i++)
    {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        cout << " " << processes[i] << "\t\t" << bt[i] << "\t " << wt[i] << "\t\t " << tat[i] << endl;
    }

    cout << "Turn around time (TAT) is = " << total_tat << endl;
    cout << "Waiting time (WT) is = " << total_wt << endl;

    cout << "Average waiting time = " << (float)total_wt / (float)n << endl;
    cout << "Average turn around time = " << (float)total_tat / (float)n << endl;
    cout << "--------------------------------------------------------------------\n\n"
         << endl;
}
void printGanttChart(int processes[], int n, int bt[], int wt[])
{
    cout << "\nGantt Chart:\n";

    // Print the top bar
    cout << " ";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < bt[i]; j++)
            cout << "--";
        cout << " ";
    }
    cout << endl;

    // Print the process ID in the middle
    cout << "|";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < bt[i] - 1; j++)
            cout << " ";
        cout << "P" << processes[i];
        for (int j = 0; j < bt[i] - 1; j++)
            cout << " ";
        cout << "|";
    }
    cout << endl;

    // Print the bottom bar
    cout << " ";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < bt[i]; j++)
            cout << "--";
        cout << " ";
    }
    cout << endl;

    // Print the waiting times
    cout << "0";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < bt[i]; j++)
            cout << "  ";
        if (wt[i] + bt[i] > 9)
            cout << "\b"; // backspace for proper formatting
        cout << wt[i] + bt[i];
    }
    cout << endl;
}

int main()
{
    int n;
    cout << "\n\nEnter the number of processes: ";
    cin >> n;

    int processes[n];
    for (int i = 0; i < n; i++)
    {
        cout << "Enter the process ID for process " << i + 1 << ": ";
        cin >> processes[i];
    }

    int burst_time[n];
    for (int i = 0; i < n; i++)
    {
        cout << "Enter the burst time for process " << processes[i] << ": ";
        cin >> burst_time[i];
    }

    findavgTime(processes, n, burst_time);
    int wt[n];
    printGanttChart(processes, n, burst_time, wt);

    return 0;
}


