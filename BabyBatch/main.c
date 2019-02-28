/* Program Assignment 0 - Batch Simulation Program */
/* Author: Christopher Linton */
/* Due Date: Feb 27, 2019 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>             /* system() */
#include <unistd.h>             /* sleep() */
#include <dirent.h>             /* directory type and methods */

#define LINE_CAP 128
#define COMMAND_CAP 512
#define PATH_CAP 256            /* assuming path is no longer than 256 char */
#define JOB_LIST_LEN 512        /* assuming there are no more than 512 jobs */
#define SLEEP_TIMEOUT 2         /* sleep timeout in seconds */

/* External variables */
char job_path[PATH_CAP + 1];
char *job_list[JOB_LIST_LEN + 1];

/* Prototypes */
int next_line(char *line, int n);
void help();
void list();
void set(char *pathname);
void run(char *filename);
void run_job(char *jobname);
bool in_job_list(char *jobname);
void shutdown();

int main(void)
{
    printf("Batch Simulation Monitor v1.0    (c) 2019, Christopher Linton\n");
    printf("Type 'help' to list commands.\n");

    for (;;) {
        char line[LINE_CAP + 1];
        char *command, *arg;
        command = arg = NULL;

        printf("\n=> ");
        next_line(line, LINE_CAP);

        command = strtok(line, " \t");
        arg = strtok(NULL, " \t");

        if (command != NULL) {
            if (strcmp("help", command) == 0)
                help();
            else if (strcmp("list", command) == 0)
                list();
            else if (strcmp("set", command) == 0)
                set(arg);
            else if (strcmp("run", command) == 0)
                run(arg);
            else if (strcmp("shutdown", command) == 0) {
                shutdown();
                break;
            } else
                printf("Invalid command.\n");
        }
    }

    return 0;
}

int next_line(char *line, int n)
{
    int ch, i = 0;

    while ((ch = getchar()) != '\n')
        if (i < n)
            line[i++] = ch;
    line[i] = '\0';

    return i;
}

/*
 * Help displays information for the monitor commands.
 */
void help()
{
    const char *help_text = "\
Command Help\n\
------------\n\
list            : Displays a list of jobs in job pool.\n\
set [pathname]  : Set the path of the job pool.\n\
run             : Compiles and executes all jobs in the job pool.\n\
run [filename]  : Compiles and executes single job from the job pool.\n\
shutdown        : Shuts the monitor down.";

    printf("%s\n", help_text);
}

/*
 * List displays the contents of the job_list array .
 *
 * List requires that the job_path/job_list be initialized.
 */
void list()
{
    if (strlen(job_path) == 0) {
        printf("The job path is not set. Run 'set [pathname]'.\n");
        return;
    }

    printf("Listing jobs in the job pool...\n\n");
    sleep(SLEEP_TIMEOUT);
    printf("Jobs by filename\n");
    printf("----------------\n");
    for (int i = 0; job_list[i] != NULL; i++)
        printf("%s\n", job_list[i]);
}

/*
 * Set requires a string argument that represents the pathname to the job_path.
 * The job_path is set from the argument and the job_list is populated
 * with the files in the directory of the job_path, not including: '.', '..',
 * 'a.out'.
 */
void set(char *pathname)
{
    if (pathname == NULL) {
        printf("Job path not given. Run 'set [pathname]'.\n");
        return;
    }

    /* set job_path */
    if (pathname[0] == '.' || pathname[0] == '/')
        sprintf(job_path, "%s", pathname);
    else
        sprintf(job_path, "./%s", pathname);

    /* set job_list */
    DIR *directory;
    struct dirent *entry;
    int i = 0;

    if ((directory = opendir(job_path)) != NULL) {
        while ((entry = readdir(directory)) != NULL)
            if (entry->d_name[0] != '.' && strcmp("a.out", entry->d_name) != 0)
                if (i < JOB_LIST_LEN)
                    job_list[i++] = entry->d_name;
        job_list[i] = NULL;
        closedir(directory);
    } else {
        printf("Invalid job path. Run 'set [pathname]'.\n");
    }
}

/*
 * Run takes a string argument that represents a single job in the job_list.
 *
 * Run requires that the job_path/job_list be initialized.
 *
 * When a filename is not supplied, Run compiles and executes all files in the
 * job_list.
 *
 * When a filename is supplied, Run compiles and executes the single job
 * as long as it is in the job_list and it is given as a valid filename.
 */
void run(char *filename)
{
    if (strlen(job_path) == 0) {
        printf("The job path is not set. Run 'set [pathname]'.\n");
        return;
    }

    if (filename == NULL) {
        for (int i = 0; job_list[i] != NULL; i++)
            run_job(job_list[i]);
    } else if (strchr(filename, '.') != NULL) {
        if (in_job_list(filename))
            run_job(filename);
        else
            printf("Invalid job. Chose job from 'list'.\n");
    } else {
        printf("'%s' not found. Specify full filename.\n", filename);
    }
}

void run_job(char *jobname)
{
    char command[COMMAND_CAP + 1];

    sprintf(command, "cc -o %s/%s %s/%s && %s/%s",
        job_path, "a.out",
        job_path, jobname,
        job_path, "a.out");

    printf("\nCompiling and running job: '%s'\n", jobname);
    sleep(SLEEP_TIMEOUT);
    system(command);
}

bool in_job_list(char *jobname) {
    for (int i = 0; job_list[i] != NULL; i++) {
        if (strcmp(job_list[i], jobname) == 0)
            return true;
    }
    return false;
}

void shutdown()
{
    printf("Shutting down...\n");
    sleep(SLEEP_TIMEOUT);
    printf("Goodbye!\n");
}
