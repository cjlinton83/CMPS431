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
};

int load_process_buffer(struct process processes[], int n);
void show_process_buffer(struct process processes[], int n);

int main(void)
{
    struct process process_buffer[BUFFER_CAP];
    int process_count;

    if ((process_count = load_process_buffer(process_buffer, BUFFER_CAP)) != 0) {
        printf("PROCESS COUNT: %d\n", process_count);
        show_process_buffer(process_buffer, process_count);
    }

    return 0;
}

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
}