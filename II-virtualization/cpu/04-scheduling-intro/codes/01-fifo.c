#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

/*
FIFO - First In, First Out

Assumption: Every task start at the same time
*/

long total_turnaround_time = 0;
long total_task = 0;
long global_time = 0;

void simulate_cpu_operation(int pid, int time) {
    global_time += time;
    printf("(pid - %d), process starting...\n", pid);
    sleep(1);
    printf("(pid - %d), process completed. Turnaround time - %ld\n", pid, global_time);
    total_task++;
    total_turnaround_time += global_time;
}

unsigned is_valid_integer(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    char *endptr;
    strtol(str, &endptr, 10);

    return *endptr == '\0';
}

void simulate_fifo(int tasks[], int len) {
    for (int i = 0; i < len; i++) {
        simulate_cpu_operation(i + 1, tasks[i]);
    }
}

void summarise() {
    printf("Total turnaround time - %ld\n", total_turnaround_time);
    printf("Avarage turnaround time - %.2f\n", ((double) total_turnaround_time) / ((double) total_task));
}

int main(int argv, char** argc) {
    if (argv > 1) {
        int *tasks = (int*) malloc(sizeof(int) * (argv - 1));
        assert(tasks != NULL);
        for (int i = 1; i < argv; i++) {
            if (!is_valid_integer(argc[i])) {
                fprintf(stderr, "usage: ./fifo 10 23 50\ntask completion time in seconds(integer)\n");
                exit(1);
            }
            int time = atoi(argc[i]);
            if (time < 0) {
                fprintf(stderr, "Only positive integers are allowed\n");
                exit(1);
            }
            tasks[i - 1] = time;
        }
        simulate_fifo(tasks, argv - 1);
        free(tasks);
    } else {
        int default_tasks[3] = {12, 30, 45};
        simulate_fifo(default_tasks, 3);
    }

    summarise();
    return 0;
}