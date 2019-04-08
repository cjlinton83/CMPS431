#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <deque>
#include <algorithm>

#define BUFFER_CAP 32

using namespace std;

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int first_in_run;
    int complete;
} Process;

int load_new_processes(Process processes[], int n);
void show_new_processes(Process processes[], int n);
void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void rr(Process processes[], int n);
void show_stats(Process processes[], int n);

int main()
{
    Process new_processes[BUFFER_CAP];
    int process_count = 0;

    cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.precision(2);

    if ((process_count = load_new_processes(new_processes, BUFFER_CAP)) != 0) {
        fcfs(new_processes, process_count);
        sjf(new_processes, process_count);
        rr(new_processes, process_count);
    }

    return 0;
}

int load_new_processes(Process processes[], int n)
{
    int i = 0;
    fstream fs("./input", fstream::in);
    string line;

    if (fs.is_open()) {
        getline(fs, line);
        while (getline(fs, line)) {
            istringstream s_stream(line);
            while (!s_stream.eof()) {
                s_stream >> processes[i].id;
                s_stream >> processes[i].arrival;
                s_stream >> processes[i].burst;
                s_stream >> processes[i].priority;
                i++;
            }
        }
        fs.close();
    }

    return i;
}

void show_new_processes(Process processes[], int n)
{
    cout << "PID\tARRIVE\tBURST\tPRIORITY" << endl;
    cout << "---\t------\t-----\t--------" << endl;

    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "\t";
        cout << processes[i].arrival << "\t";
        cout << processes[i].burst << "\t";
        cout << processes[i].priority << "\t";
        cout << endl;
    }

    cout << "\nNumber of jobs in NewQ = " << n << endl << endl;
}

void show_stats(Process processes[], int n)
{
    float throughput, turnaround, response, total_burst;
    throughput = turnaround = response = total_burst = 0.0f;

    cout << "PID\tARRIVE\tCOMPLETE" << endl;
    cout << "---\t------\t--------" << endl;

    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "\t";
        cout << processes[i].arrival << "\t";
        cout << processes[i].complete << "\t";
        cout << endl;

        turnaround += (processes[i].complete - processes[i].arrival);
        response += (processes[i].first_in_run - processes[i].arrival);
        total_burst += processes[i].burst;
    }

    cout << "\nRun Stats" << endl;
    cout << "Throughput = " << (float)n / total_burst << endl;
    cout << "Average turnaround time = " << turnaround / (float)n << endl;
    cout << "Average response time = " << response / (float) n << endl << endl;
}

bool sort_arrive(Process *p1, Process *p2) {
    return (p1->arrival < p2->arrival);
}

bool sort_ready(Process *p1, Process *p2) {
    return (p1->burst < p2->burst);
}

void fcfs(Process processes[], int n)
{

}

void sjf(Process processes[], int n)
{
    int total_time = 0;

    deque<Process *> arriveQ;
    deque<Process *> readyQ;

    // Show newQ
    show_new_processes(processes, n);

    // setup and perform SJF
    for (int i = 0; i < n; i++)
        arriveQ.push_back(processes + i);

    sort(arriveQ.begin(), arriveQ.end(), sort_arrive);

    while (!arriveQ.empty() || !readyQ.empty()) {

        if (!arriveQ.empty())
            if (arriveQ.front()->arrival <= total_time) {
                readyQ.push_back(arriveQ.front());
                arriveQ.pop_front();
                continue;
            }


        if (!readyQ.empty()) {
            sort(readyQ.begin(), readyQ.end(), sort_ready);

            readyQ.front()->first_in_run = total_time;
            total_time += readyQ.front()->burst;
            readyQ.front()->complete = total_time;
            readyQ.pop_front();
        }
    }

    // Show stats
    cout << "Terminated Jobs. (Shortest Job First)" << endl;
    show_stats(processes, n);
}

void rr(Process processes[], int n)
{

}
