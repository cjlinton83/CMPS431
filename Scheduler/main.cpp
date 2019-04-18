#include <stdio.h>
#include <string>
#include <deque>

#define LINE_CAP 80

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int working_burst;
    int start;
    int finished;
} Process;

int load_new_queue(std::deque<Process *> *newQ);
void show_queue(std::deque<Process *> newQ, int n);
void clear_stats(std::deque<Process *> *newQ, int n);
void show_stats(std::deque<Process *> processQ, int n, std::string name);
std::deque<Process *> fcfs(std::deque<Process *> *newQ, int n);
std::deque<Process *> sjf(std::deque<Process *> *newQ, int n);
std::deque<Process *> rr(std::deque<Process *> *newQ, int n, int quantum);

int main()
{
    std::deque<Process *> newQ;
    std::deque<Process *> finishedQ;
    int n;

    if ((n = load_new_queue(&newQ)) > 0) {
        show_queue(newQ, n);
        finishedQ = fcfs(&newQ, n);
        show_stats(finishedQ, n, "First Come First Serve");

        show_queue(newQ, n);
        finishedQ = sjf(&newQ, n);
        show_stats(finishedQ, n, "Shortest Job First");

        show_queue(newQ, n);
        finishedQ = rr(&newQ, n, 15);
        show_stats(finishedQ, n, "Round Robin");
    }
    
    return 0;
}

int load_new_queue(std::deque<Process *> *newQ)
{
    int n = 0;
    FILE *fp = fopen("input", "r");
    char line[LINE_CAP+1];

    if (fp != NULL) {
        fgets(line, LINE_CAP, fp); // remove header
        while (fgets(line, LINE_CAP, fp) != NULL) {
            int id, arrival, burst, priority;

            sscanf(line, "%d %d %d %d", &id, &arrival, &burst, &priority);
            Process *p = (Process *) malloc(sizeof(Process));
            p->id = id;
            p->arrival = arrival;
            p->burst = burst;
            p->priority = priority;
            p->working_burst = burst;
            p->start = -1;
            p->finished = -1;
            newQ->push_back(p);
            n++;
        }
        fclose(fp);
    }

    return n;
}

void show_queue(std::deque<Process *> newQ, int n)
{
    printf("PID\tARRIVAL\tBURST\tPRIORITY\n");
    printf("---\t-------\t-----\t--------\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\n",
            newQ[i]->id, newQ[i]->arrival,
            newQ[i]->burst, newQ[i]->priority);
    }

    printf("Number of jobs in queue: %d\n\n", n);
}

void clear_stats(std::deque<Process *> *newQ, int n)
{
    for (int i = 0; i < n; i++) {
        (*newQ)[i]->working_burst = (*newQ)[i]->burst;
        (*newQ)[i]->start = -1;
        (*newQ)[i]->finished = -1;
    }
}

void show_stats(std::deque<Process *> processQ, int n, std::string name)
{
    float throughput, turnaround, response, total_burst;
    throughput = turnaround = response = total_burst = 0.0f;

    printf("Terminated jobs (%s)\n", name.c_str());
    printf("PID\tARRIVAL\tCOMPLETE\n");
    printf("---\t-------\t--------\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n",
            processQ[i]->id, processQ[i]->arrival, processQ[i]->finished);

        turnaround += (processQ[i]->finished - processQ[i]->arrival);
        response += (processQ[i]->start - processQ[i]->arrival);
        total_burst += processQ[i]->burst;
    }

    throughput = (float)n / total_burst;
    turnaround /= (float)n;
    response /= (float)n;
    
    printf("Run Stats:\n");
    printf("Throughput = %.2f\n", throughput);
    printf("Average turnaround time = %.2f\n", turnaround);
    printf("Average response time = %.2f\n\n", response);
}

bool sort_arrival(Process *p1, Process *p2)
{
    return p1->arrival < p2->arrival;
}

std::deque<Process *> fcfs(std::deque<Process *> *newQ, int n)
{
    int total_time = 0;

    std::deque<Process *> readyQ;
    std::deque<Process *> runQ;
    std::deque<Process *> finishedQ;

    clear_stats(newQ, n);

    for (int i = 0; i < n; i++) {
        readyQ.push_back((*newQ)[i]);
    }
    std::sort(newQ->begin(), newQ->end(), sort_arrival);

    while (!readyQ.empty() || !runQ.empty()) {
        if (!readyQ.empty()) {
            if (readyQ.front()->arrival <= total_time) {
                Process *p = readyQ.front();
                readyQ.pop_front();
                runQ.push_back(p);
                continue;
            }
        }

        if (!runQ.empty()) {
            Process *p = runQ.front();
            runQ.pop_front();

            p->start = total_time;
            total_time += p->burst;
            p->finished = total_time;
            finishedQ.push_back(p);
        }
    }
    
    return finishedQ;
}

bool sort_burst(Process *p1, Process *p2)
{
    return p1->burst < p2->burst;
}

std::deque<Process *> sjf(std::deque<Process *> *newQ, int n)
{
    int total_time = 0;

    std::deque<Process *> readyQ;
    std::deque<Process *> runQ;
    std::deque<Process *> finishedQ;

    clear_stats(newQ, n);

    for (int i = 0; i < n; i++) {
        readyQ.push_back((*newQ)[i]);
    }
    std::sort(newQ->begin(), newQ->end(), sort_arrival);

    while (!readyQ.empty() || !runQ.empty()) {
        if (!readyQ.empty()) {
            if (readyQ.front()->arrival <= total_time) {
                Process *p = readyQ.front();
                readyQ.pop_front();
                runQ.push_back(p);
                continue;
            }
        }

        if (!runQ.empty()) {
            std::sort(runQ.begin(), runQ.end(), sort_burst);
            Process *p = runQ.front();
            runQ.pop_front();

            p->start = total_time;
            total_time += p->burst;
            p->finished = total_time;
            finishedQ.push_back(p);
        }
    }
    
    return finishedQ;
}

std::deque<Process *> rr(std::deque<Process *> *newQ, int n, int quantum)
{
    int total_time = 0;

    std::deque<Process *> readyQ;
    std::deque<Process *> runQ;
    std::deque<Process *> finishedQ;

    clear_stats(newQ, n);

    for (int i = 0; i < n; i++) {
        readyQ.push_back((*newQ)[i]);
    }
    std::sort(newQ->begin(), newQ->end(), sort_arrival);

    while (!readyQ.empty() || !runQ.empty()) {
        if (!readyQ.empty()) {
            if (readyQ.front()->arrival <= total_time) {
                Process *p = readyQ.front();
                readyQ.pop_front();
                runQ.push_back(p);
                continue;
            }
        }

        if (!runQ.empty()) {
            Process *p = runQ.front();
            runQ.pop_front();

            if (p->start < 0) {
                p->start = total_time;
            }

            if (p->working_burst <= quantum) {
                total_time += p->working_burst;
                p->finished = total_time;
                finishedQ.push_back(p);
            } else {
                total_time += quantum;
                p->working_burst -= quantum;
                runQ.push_back(p);
            }
        }
    }
    
    return finishedQ;
}