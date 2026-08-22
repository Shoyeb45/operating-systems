#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
RR - Round Robin algorithm
*/

enum proc_status {
    NEW,
    READY,
    RUNNING,
    COMPLETED,
};

const int RQ_LEN = 1200;
int ready_queue[RQ_LEN];
int rq_front = 0;
int rq_back = 0;
int rq_size = 0;

int rq_empty() {
    return rq_size == 0;
}

void enqueue(int element) {
    ready_queue[rq_back] = element;
    rq_size++;
    rq_back = (rq_back + 1) % RQ_LEN;
}

int dequeue() {
    int ele = ready_queue[rq_front];
    rq_front = (rq_front + 1) % RQ_LEN;
    rq_size--;
    return ele;
}

typedef struct __proc_info {
    int pid;
    long arrival_time;
    long job_time;
    long completion_time;
    long remaining_time;
    long start_time;
    enum proc_status status;
} proc_info;

proc_info *processes;
int total_processes = 0;
long TIME_SHARING = 10;

// using O(n^2) algo because this is just for simulation purpose
void sort_process(unsigned is_arrival_sort) {
    for (int i = 0; i < total_processes; i++) {
        for (int j = i + 1; j < total_processes; j++) {
            long pi = is_arrival_sort ? processes[i].start_time : processes[i].arrival_time;
            long pj = is_arrival_sort ? processes[j].start_time : processes[j].arrival_time;
            if (pj < pi) {
                proc_info temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

long min(long a, long b) {
    return a < b ? a : b;
}

void simulate_cpu_execution(long *current_time, int *proc_completed) {
    int idx = dequeue();

    processes[idx].status = RUNNING;
    printf("[P%d][%ld] - READY -> RUNNING\n", processes[idx].pid, *current_time);

    if (processes[idx].remaining_time == processes[idx].job_time) {
        processes[idx].start_time = *current_time;
    }

    long time = min(TIME_SHARING, processes[idx].remaining_time);
    processes[idx].remaining_time -= time;
    *current_time += time;

    if (processes[idx].remaining_time == 0) {
        processes[idx].status = COMPLETED;
        processes[idx].completion_time = *current_time;
        *proc_completed += 1;
        printf("[P%d][%ld] - RUNNING -> COMPLETED\n", processes[idx].pid, *current_time);
    } else {
        processes[idx].status = READY;
        enqueue(idx);
        printf("[P%d][%ld] - RUNNING -> READY\n", processes[idx].pid, *current_time);
    }
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

void simulate_rr() {
    sort_process(0);

    int process_completed = 0;
    long current_time = 0;
    int idx = 0;
    while (process_completed < total_processes) {
        int found_proc = 0;
        for (int i = 0; i < total_processes; i++) {
            if (processes[i].status == NEW && processes[i].arrival_time <= current_time) {
                processes[i].status = READY;
                printf("[P%d][%ld] - NEW -> READY\n", processes[i].pid, current_time);
                enqueue(i);
            }
        }
        if (rq_empty()) {
            printf("[%ld] IDLE CPU\n", current_time);
            current_time++;
            continue;
        }
        simulate_cpu_execution(&current_time, &process_completed);
    }
}

void summarise() {
    printf("\nSummary\n\n");
    sort_process(1);
    long total_turnaround_time = 0;
    long total_response_time = 0;
    printf("|PID\t| Arrival Time  | Start Time\t| Completion Time | Job Time\t|\n");
    printf("|-------|---------------|---------------|-----------------|-------------|\n");
    for (int i = 0; i < total_processes; i++) {
        printf("|%d\t| %ld\t\t| %ld\t\t| %ld\t\t  | %ld\t\t|\n", processes[i].pid, processes[i].arrival_time,
               processes[i].start_time, processes[i].completion_time, processes[i].job_time);
        total_turnaround_time += processes[i].completion_time - processes[i].arrival_time;
        total_response_time += processes[i].start_time - processes[i].arrival_time;
    }

    printf("|_______|_______________|_______________|_________________|_____________|\n\n");
    printf("Total turnaround time - %ld\n", total_turnaround_time);
    printf("Avarage turnaround time - %.2f\n", ((double)total_turnaround_time) / ((double)total_processes));
    printf("Avarage response time - %.2f\n", ((double)total_response_time) / ((double)total_processes));
}

void usage() {
    fprintf(
        stderr,
        "usage: ./rr <arrival_time job_time>[] TIME_SHARING\nEx., ./sjb 0 100 10 10 10 10 10\nIt means, we have P1 - "
        "arrived at 0 with job time 100 and so on with time sharing 10");
    exit(1);
}

void proc_details() {
    printf("Job Details\n");
    printf("|PID\t| Arrival Time  | Job Time\t|\n");
    printf("|-------|---------------|---------------|\n");
    for (int i = 0; i < total_processes; i++) {
        printf("|%d\t| %ld\t\t| %ld\t\t|\n", processes[i].pid, processes[i].arrival_time, processes[i].job_time);
    }
    printf("|_______|_______________|_______________|\n");
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc > 1) {
        int k = argc - 2;
        if ((k & 1)) {
            usage();
        }
        if (k / 2 > 1200) {
            fprintf(stderr, "The number of process must be less than or equal to 1200\n");
            exit(1);
        }
        processes = (proc_info *)calloc(k / 2, sizeof(proc_info));
        assert(processes != NULL);

        // ./arg 0 12 0 34 0 123
        for (int i = 0; i < k / 2; i++) {
            long arrival_time = get_time(argv[2 * i + 1]);
            long job_time = get_time(argv[2 * i + 2]);
            processes[i].arrival_time = arrival_time;
            processes[i].job_time = job_time;
            processes[i].remaining_time = job_time;
            processes[i].pid = i;
        }
        total_processes = k / 2;
        TIME_SHARING = get_time(argv[argc - 1]);
    } else {
        // initialize default
        processes = (proc_info *)calloc(3, sizeof(proc_info));
        assert(processes != NULL);

        processes[0].arrival_time = 0;
        processes[0].job_time = 100;
        processes[0].remaining_time = 100;
        processes[0].pid = 0;

        processes[1].arrival_time = 10;
        processes[1].job_time = 10;
        processes[1].remaining_time = 10;
        processes[1].pid = 1;

        processes[2].arrival_time = 10;
        processes[2].job_time = 10;
        processes[2].remaining_time = 10;
        processes[2].pid = 2;

        total_processes = 3;
    }

    proc_details();
    simulate_rr();
    summarise();
    free(processes);
    return 0;
}