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

int load_new_processes();
void show_new_processes();
void fcfs();
void sjf();
void rr();
void show_stats();

Process processes[BUFFER_CAP];
int n = 0;

int main()
{
    cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.precision(2);

    if ((n = load_new_processes()) != 0) {
        show_new_processes();
        fcfs();
        show_stats();

        show_new_processes();
        sjf();
        show_stats();

        // show_new_processes();
        // rr();
        // show_stats();
    }

    return 0;
}

int load_new_processes()
{
    int i = 0;
    fstream fs("./input", fstream::in);
    string line;

    if (fs.is_open()) {
        getline(fs, line);
        while (getline(fs, line)) {
            if (i < BUFFER_CAP) {
                istringstream s_stream(line);
                while (!s_stream.eof()) {
                    s_stream >> processes[i].id;
                    s_stream >> processes[i].arrival;
                    s_stream >> processes[i].burst;
                    s_stream >> processes[i].priority;
                    i++;
                }
            }
        }
        fs.close();
    }

    return i;
}

void show_new_processes()
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

    cout << "Number of jobs in NewQ = " << n << endl << endl;
}

void show_stats()
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

    cout << "Run Stats" << endl;
    cout << "Throughput = " << (float)n / total_burst << endl;
    cout << "Average turnaround time = " << turnaround / (float)n << endl;
    cout << "Average response time = " << response / (float)n << endl << endl;
}

bool sort_arrive(Process *p1, Process *p2) {
    return (p1->arrival < p2->arrival);
}

bool sort_ready(Process *p1, Process *p2) {
    return (p1->burst < p2->burst);
}

void fcfs()
{
    int total_time = 0;

    deque<Process *> arriveQ;
    deque<Process *> readyQ;

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
            readyQ.front()->first_in_run = total_time;
            total_time += readyQ.front()->burst;
            readyQ.front()->complete = total_time;
            readyQ.pop_front();
        }
    }

    cout << "Terminated Jobs. (First Come, First Serve)" << endl;
}

void sjf()
{
    int total_time = 0;

    deque<Process *> arriveQ;
    deque<Process *> readyQ;

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

    cout << "Terminated Jobs. (Shortest Job First)" << endl;
}

void rr()
{

}
