#include <bits/stdc++.h>
#include <queue>
#include <map>
using namespace std;
#define INT16_MAX (32767)

class Job
{
    string proc;
    int p_id;
    int burst_time;
    int arr_time;
    int wait_time;
    int turn_around_time;
    int comp_time;
    int start_time;
    int prio;
    friend bool compare(Job, Job);

public:
    Job()
    {
        proc = "";
        p_id = 0;
        burst_time = 0;
        arr_time = 0;
        wait_time = 0;
        turn_around_time = 0;
        comp_time = 0;
        start_time = 0;
        prio = 0;
    }
    void input(int a)
    {
        cout << "Enter the name of the process: ";
        cin >> proc;
        cout << "Enter the burst time: ";
        cin >> burst_time;
        cout << "Enter arrival time: ";
        cin >> arr_time;
        p_id = a;
    }
    friend class Schedule;
};

bool compare(Job a, Job b)
{
    if (a.arr_time == b.arr_time && a.prio == b.prio)
    {
        return a.p_id < b.p_id;
    }
    else if (a.arr_time == b.arr_time)
    {
        return a.prio < b.prio;
    }
    else
    {
        return a.arr_time < b.arr_time;
    }
}

class Schedule
{
    Job *arr;
    int n;

public:
    Schedule()
    {
        arr = NULL;
        n = 0;
    }
    void get_Job()
    {
        cout << "Enter the number of processes: ";
        cin >> n;
        arr = new Job[n];
        for (int i = 0; i < n; i++)
        {
            arr[i].input(i + 1);
        }
    }
    void display(Job arr[])
    {
        cout << "Name\tID\tBT\tAT\tCT\tWT\tTAT" << endl;
        for (int i = 0; i < n; i++)
        {
            cout << arr[i].proc << "\t" << arr[i].p_id << "\t" << arr[i].burst_time << "\t" << arr[i].arr_time << "\t" << arr[i].comp_time << "\t" << arr[i].wait_time << "\t" << arr[i].turn_around_time << endl;
        }
    }
    void displayGantt(vector<tuple<string, int, int>> &gantt)
    {
        cout << "\nGantt Chart:\n";
        for (auto &segment : gantt)
        {
            cout << get<0>(segment) << " [" << get<1>(segment) << " - " << get<2>(segment) << "] ";
        }
        cout << endl;
    }
    Job *sorting()
    {
        int min;
        Job *temp = arr;
        for (int i = 0; i < n; i++)
        {
            temp[i] = arr[i];
        }
        for (int i = 0; i < n; i++)
        {
            min = i;
            for (int j = i + 1; j < n; j++)
            {
                if (temp[i].arr_time > temp[j].arr_time)
                {
                    min = j;
                }
                else if (temp[i].arr_time == temp[j].arr_time)
                {
                    if (temp[i].p_id > temp[j].p_id)
                    {
                        min = j;
                    }
                }
            }
            if (min != i)
            {
                swap(temp[i], temp[min]);
            }
        }
        return temp;
    }
    void FCFS()
    {
        Job *temp = sorting();
        int prev = 0;
        vector<tuple<string, int, int>> gantt;
        for (int i = 0; i < n; i++)
        {
            temp[i].start_time = max(prev, temp[i].arr_time);
            temp[i].comp_time = temp[i].burst_time + temp[i].start_time;
            temp[i].turn_around_time = temp[i].comp_time - temp[i].arr_time;
            temp[i].wait_time = temp[i].turn_around_time - temp[i].burst_time;
            prev = temp[i].comp_time;
            gantt.push_back(make_tuple(temp[i].proc, temp[i].start_time, temp[i].comp_time));
        }
        int sum = 0;
        float sum1 = 0;
        for (int k = 0; k < n; k++)
        {
            sum += temp[k].wait_time;
            sum1 += temp[k].turn_around_time;
        }
        display(temp);
        displayGantt(gantt);
        cout << "_______________\nAverage waiting time: " << (sum / n) << endl;
        cout << "Average turnAround time: " << (sum1 / n) << endl;
    }

    void SJF()
    {
        Job *temp = arr;
        int remain_time[n];
        int completed = 0;
        int shortest;
        int minB = INT16_MAX;
        int t = 0;
        bool check = false;
        vector<tuple<string, int, int>> gantt;
        for (int i = 0; i < n; i++)
        {
            remain_time[i] = temp[i].burst_time;
        }
        while (completed != n)
        {
            for (int j = 0; j < n; j++)
            {
                if (temp[j].arr_time <= t && (remain_time[j] < minB && remain_time[j] > 0))
                {
                    shortest = j;
                    minB = remain_time[j];
                    check = true;
                }
            }
            if (check == false)
            {
                t++;
                continue;
            }
            if (remain_time[shortest] == temp[shortest].burst_time)
            {
                temp[shortest].start_time = t;
            }
            remain_time[shortest]--;
            minB = remain_time[shortest];
            if (minB == 0)
            {
                minB = INT16_MAX;
            }
            if (remain_time[shortest] == 0)
            {
                completed++;
                check = false;
                temp[shortest].comp_time = t + 1;
                temp[shortest].wait_time = temp[shortest].comp_time - temp[shortest].burst_time - temp[shortest].arr_time;
                temp[shortest].turn_around_time = temp[shortest].burst_time + temp[shortest].wait_time;
                gantt.push_back(make_tuple(temp[shortest].proc, temp[shortest].start_time, temp[shortest].comp_time));
            }
            t++;
        }
        float sum = 0;
        float sum1 = 0;
        for (int k = 0; k < n; k++)
        {
            sum += temp[k].wait_time;
            sum1 += temp[k].turn_around_time;
        }
        display(temp);
        displayGantt(gantt);
        cout << "_______________\nAverage waiting time: " << sum / n << endl;
        cout << "Average turnAround time: " << sum1 / n << endl;
    }
    void SJF_NP()
    {
        Job *temp = arr;
        int shortest;
        int t = 0;
        int minB = INT16_MAX;
        bool check = false;
        bool completed[n];
        int count = 0;
        vector<tuple<string, int, int>> gantt;
        for (int i = 0; i < n; i++)
        {
            completed[i] = false;
        }
        while (count < n)
        {
            for (int i = 0; i < n; i++)
            {
                if (temp[i].arr_time <= t && temp[i].burst_time < minB && completed[i] == false)
                {
                    minB = temp[i].burst_time;
                    shortest = i;
                    check = true;
                }
            }
            if (check == false)
            {
                t++;
                continue;
            }
            minB = INT16_MAX;
            temp[shortest].start_time = t;
            temp[shortest].wait_time = t - temp[shortest].arr_time;
            t += temp[shortest].burst_time;
            temp[shortest].comp_time = t;
            temp[shortest].turn_around_time = temp[shortest].wait_time + temp[shortest].burst_time;
            completed[shortest] = true;
            count++;
            gantt.push_back(make_tuple(temp[shortest].proc, temp[shortest].start_time, temp[shortest].comp_time));
        }
        float sum = 0;
        float sum1 = 0;
        for (int k = 0; k < n; k++)
        {
            sum += temp[k].wait_time;
            sum1 += temp[k].turn_around_time;
        }
        display(temp);
        displayGantt(gantt);
        cout << "_______________\nAverage waiting time: " << sum / n << endl;
        cout << "Average TurnAround time: " << sum1 / n << endl;
    }

    void Round_Robin()
    {
        queue<int> q;
        bool visited[100] = {false};
        int current_time = 0;
        int completed = 0;
        int sum_tat = 0;
        int sum_wt = 0;
        int index;
        Job *temp = sorting();
        int tq;
        cout << "Enter Time quantum: ";
        cin >> tq;
        int remain_time[n];
        vector<tuple<string, int, int>> gantt;
        for (int i = 0; i < n; i++)
        {
            remain_time[i] = temp[i].burst_time;
        }
        q.push(0);
        visited[0] = true;
        while (completed != n)
        {
            index = q.front();
            q.pop();
            if (remain_time[index] == temp[index].burst_time)
            {
                temp[index].start_time = max(current_time, temp[index].arr_time);
                current_time = temp[index].start_time;
            }
            if (remain_time[index] - tq > 0)
            {
                remain_time[index] -= tq;
                current_time += tq;
            }
            else
            {
                current_time += remain_time[index];
                remain_time[index] = 0;
                completed++;

                temp[index].comp_time = current_time;
                temp[index].turn_around_time = temp[index].comp_time - temp[index].arr_time;
                temp[index].wait_time = temp[index].turn_around_time - temp[index].burst_time;

                sum_tat += temp[index].turn_around_time;
                sum_wt += temp[index].wait_time;
                gantt.push_back(make_tuple(temp[index].proc, temp[index].start_time, temp[index].comp_time));
            }

            for (int i = 1; i < n; i++)
            {
                if (remain_time[index] > 0 && temp[i].arr_time <= current_time && visited[i] == false)
                {
                    q.push(i);
                    visited[i] = true;
                }
            }

            if (remain_time[index] > 0)
                q.push(index);

            if (q.empty())
            {
                for (int i = 1; i < n; i++)
                {
                    if (remain_time[index] > 0)
                    {
                        q.push(i);
                        visited[i] = true;
                        break;
                    }
                }
            }
        }

        display(temp);
        displayGantt(gantt);
        float avg;
        avg = sum_wt / n;
        cout << "Average waiting time " << avg << endl;
        cout << "Average turnAround time: " << sum_tat / n << endl;
    }

    void priority()
    {
        Job *temp = arr;

        // Input priorities
        cout << "Enter priorities for each process:" << endl;
        for (int i = 0; i < n; i++)
        {
            cout << "Enter priority for " << temp[i].proc << ": ";
            cin >> temp[i].prio;
        }

        // Sort jobs based on priority using the compare function
        sort(temp, temp + n, compare);

        // Calculate completion time, waiting time, and turnaround time
        vector<tuple<string, int, int>> gantt;
        temp[0].comp_time = temp[0].arr_time + temp[0].burst_time;
        temp[0].wait_time = 0;
        temp[0].turn_around_time = temp[0].comp_time - temp[0].arr_time;
        temp[0].start_time = temp[0].arr_time;
        gantt.push_back(make_tuple(temp[0].proc, temp[0].start_time, temp[0].comp_time));
        for (int i = 1; i < n; i++)
        {
            temp[i].start_time = max(temp[i - 1].comp_time, temp[i].arr_time);
            temp[i].comp_time = temp[i].burst_time + temp[i].start_time;
            temp[i].turn_around_time = temp[i].comp_time - temp[i].arr_time;
            temp[i].wait_time = temp[i].turn_around_time - temp[i].burst_time;
            if (temp[i].wait_time < 0)
            {
                temp[i].wait_time = 0;
            }
            gantt.push_back(make_tuple(temp[i].proc, temp[i].start_time, temp[i].comp_time));
        }

        // Calculate average waiting time
        float sum = 0;
        float sum1 = 0;
        for (int i = 0; i < n; i++)
        {
            sum += temp[i].wait_time;
            sum1 += temp[i].turn_around_time;
        }
        display(temp);
        displayGantt(gantt);
        cout << "Average waiting time is " << sum / n << endl;
        cout << "Average turnAround time is " << sum1 / n << endl;
    }
};

int main()
{
    cout << "!!!Hello World!!!" << endl;
    Schedule S;
    S.get_Job();
    cout << endl
         << "FCFS" << endl;
    S.FCFS();
    cout << endl
         << "Round Robin" << endl;
    S.Round_Robin();
    cout << endl
         << "SJF" << endl;
    S.SJF();
    cout << endl
         << "Priority" << endl;
    S.priority();

    return 0;
}

