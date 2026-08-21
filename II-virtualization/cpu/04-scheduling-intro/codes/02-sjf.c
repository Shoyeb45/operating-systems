#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

/*
SJB - Shortest Job First
- We will process the job in ascending order
- here every job will have different arrival time
*/

typedef struct __proc_info {
    int pid;
    long arrival_time;
    long job_time;
    long completion_time;
    long start_time;
    int completed;
} proc_info;

proc_info *processes;
int total_processes = 0;

// using O(n^2) algo because this is just for simulation purpose
void sort_process(unsigned is_arrival_sort) {
    for (int i = 0; i < total_processes; i++) {
        for (int j = i + 1; j < total_processes; j++) {
            long pi = is_arrival_sort ? processes[i].start_time : processes[i].job_time;
            long pj = is_arrival_sort ? processes[j].start_time : processes[j].job_time;
            if (pj < pi) {
                proc_info temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void simulate_cpu_execution(int idx, long *current_time) {
    processes[idx].start_time = *current_time;
    *current_time = *current_time + processes[idx].job_time;
    processes[idx].completion_time = *current_time;
    processes[idx].completed = 1;
}

long get_time(const char *str) {
    const char *err_msg = "Please provide a valid integer greater than or equal to 0.";
    if (str == NULL || *str == '\0') {
        fprintf(stderr, "%s", err_msg);
        exit(1);
    } 
    char *endptr;
    long num = strtol(str, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "%s", err_msg);
        exit(1);
    }
    return num;
}

void simulate_sjf() {
    // sort the array with their job time
    sort_process(0);

    int process_completed = 0;
    long current_time = 0;
    while (process_completed < total_processes) {
        // find first process which we can execute
        for (int i = 0; i < total_processes; i++) {
            if (!processes[i].completed && processes[i].arrival_time <= current_time) {
                simulate_cpu_execution(i, &current_time);
                process_completed++;
                break;
            }
        }
    }
}

void summarise() {
    sort_process(1);
    long total_turnaround_time = 0;
    
    printf("|PID\t| Arrival Time  | Start Time\t| Completion Time | Job Time\t|\n");
    for (int i = 0; i < total_processes; i++) {
        printf("|%d\t| %ld\t\t| %ld\t\t| %ld\t\t  | %ld\t\t|\n", processes[i].pid, processes[i].arrival_time, processes[i].start_time, processes[i].completion_time, processes[i].job_time);
        total_turnaround_time += processes[i].completion_time - processes[i].arrival_time;
    }

    printf("Total turnaround time - %ld\n", total_turnaround_time);
    printf("Avarage turnaround time - %.2f\n", ((double) total_turnaround_time) / ((double) total_processes));
}

void usage() {
    fprintf(stderr, "usage: ./sjb <arrival_time job_time>[]\nEx., ./sjb 0 100 10 10 10 10\nIt means, we have P1 - arrived at 0 with job time 100 and so on.");
    exit(1);
}


int main(int argc, char** argv) {

    if (argc > 1) {
        int k = argc - 1;
        if ((k & 1)) {
            usage();
        }
        processes = (proc_info*) calloc(k / 2, sizeof(proc_info));
        assert(processes != NULL);

        // ./arg 0 12 0 34 0 123
        for (int i = 0; i < k / 2; i++) {
            long arrival_time = get_time(argv[2 * i + 1]);
            long job_time = get_time(argv[2 * i + 2]);
            processes[i].arrival_time = arrival_time;
            processes[i].job_time = job_time;
            processes[i].pid = i;
        }
        total_processes = k / 2;
    } else {
        // initialize default
        processes = (proc_info*) calloc(3, sizeof(proc_info));
        assert(processes != NULL);

        processes[0].arrival_time = 0;
        processes[0].job_time = 100;
        processes[0].pid = 0;

        processes[1].arrival_time = 10;
        processes[1].job_time = 10;
        processes[1].pid = 1;

        processes[2].arrival_time = 10;
        processes[2].job_time = 10;
        processes[2].pid = 2;

        total_processes = 3;
    }

    simulate_sjf();
    summarise();
    free(processes);
    return 0;
}