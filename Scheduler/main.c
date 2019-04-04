#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIM           " \t"
#define BUFFER_CAP      32
#define INPUT_LINE_LEN  80

struct process {
    int id;
    int arrival;
    int burst;
    int priority;
    int first_in_queue;
    int complete;
};

int load_process_buffer(struct process processes[], int n);
void show_process_buffer(struct process processes[], int n);
void fcfs(struct process processes[], int n);
void show_process_stats(struct process processes[], int n, char *job_label);

int main(void)
{
    struct process process_buffer[BUFFER_CAP];
    int process_count;

    if ((process_count = load_process_buffer(process_buffer, BUFFER_CAP)) != 0) {
        show_process_buffer(process_buffer, process_count);
        fcfs(process_buffer, process_count);
        show_process_stats(process_buffer, process_count, "First Come, First Serve");
    }

    return 0;
}

/*
 * load_process_buffer: loads process information, from a desctiption file,
 *   into the processes buffer. The function loads up to n processes.
 *   the function returns the number of processes loaded into the buffer.
 */
int load_process_buffer(struct process processes[], int n)
{
    FILE *fp;
    fp = fopen("./input", "r");

    char input_line[INPUT_LINE_LEN+1];
    int i = 0;
    while (i < n && fgets(input_line, INPUT_LINE_LEN, fp) != NULL) {
        char *field;

        field = strtok(input_line, DELIM);
        processes[i].id = atoi(field);

        field = strtok(NULL, DELIM);
        processes[i].arrival = atoi(field);

        field = strtok(NULL, DELIM);
        processes[i].burst = atoi(field);

        field = strtok(NULL, DELIM);
        processes[i].priority = atoi(field);

        i++;
    }

    fclose(fp);

    return i;
}

void show_process_buffer(struct process processes[], int n)
{
    printf("PID\tARRIVAL\tBURST\tPRIORITY\n");
    printf("---\t-------\t-----\t--------\n");
    
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\n",
            processes[i].id,
            processes[i].arrival,
            processes[i].burst,
            processes[i].priority);
    printf("\n");
}

void fcfs(struct process processes[], int n)
{
    int in = processes[0].arrival;
    int done = processes[0].burst;

    processes[0].first_in_queue = in;
    processes[0].complete = done;

    for (int i = 1; i < n; i++) {
        in = done;
        done += processes[i].burst;
        
        processes[i].first_in_queue = in;
        processes[i].complete = done;
    }

}

void show_process_stats(struct process processes[], int n, char *job_label)
{
    int total_burst_time = 0;
    int total_turn_around = 0;
    int total_response = 0;

    printf("Number of jobs in buffer: %d\n", n);
    printf("Terminated Jobs (%s).\n", job_label);
    printf("PID\tARRIVAL\tCOMPLETION\n");
    printf("---\t-------\t----------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n",
            processes[i].id,
            processes[i].arrival,
            processes[i].complete);
        total_burst_time += processes[i].burst;
        total_turn_around += (processes[i].complete - processes[i].arrival);
        total_response += (processes[i].first_in_queue - processes[i].arrival);
    }
    
    printf("\nRun Stats:\n");
    printf("Throughtput = %.2f\n", (float)n / (float)total_burst_time);
    printf("Average turnaround time = %.2f\n", (float)total_turn_around / (float) n);
    printf("Average response time = %.2f\n\n", (float)total_response / (float)n);
}
