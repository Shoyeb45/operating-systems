#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*

MLFQ - Multi Level Feedback Queue

author - Shoyeb Ansari <shoyebff45@gmail.com>

This implementation is for learning purpose, we are going to simulate cpu, interruption, process execution, I/O request
and try to nake it like it's working in real OS with CPU and all.

We will keep the time_quanta same for each queue for simplicity

*/

// ----------  Constants -----------------------

// time period of boost
const int BOOST_PERIOD = 100;
const int QUEUE_NUM = 3;
const int TIME_SHARING = 20;

// process info
enum proc_status { NEW, READY, RUNNING, COMPLETED, IDLE, BLOCKED };
typedef struct __proc_info {
    int pid;
    
    int arrival_time;
    int run_time;
    int start_time;
    int end_time;
    int remaining_time;
    int time_allottment_rem;
    
    enum proc_status status;
    int queue_num;

    int (*io_bursts)[2];
    int io_burst_len;
    int current_io_burst;
    int io_complete_time;
    int cpu_time_completed;

} proc_info;

proc_info *processes;
int total_processes = 0;

// queue info and util functions:
const int QUEUE_LEN = 1200;
int queues[QUEUE_NUM][QUEUE_LEN];
int rq_front[QUEUE_NUM] = {0, 0, 0};
int rq_back[QUEUE_NUM] = {0, 0, 0};
int rq_size[QUEUE_NUM] = {0, 0, 0};

int empty(int queue_num) {
    assert(queue_num >= 0 && queue_num < QUEUE_NUM);
    return rq_size[queue_num] == 0;
}

void enqueue(int queue_num, int element) {
    assert(queue_num >= 0 && queue_num < QUEUE_NUM);
    queues[queue_num][rq_back[queue_num]] = element;
    rq_size[queue_num]++;
    rq_back[queue_num] = (rq_back[queue_num] + 1) % QUEUE_LEN;
}

int dequeue(int queue_num) {
    assert(queue_num >= 0 && queue_num < QUEUE_NUM);
    int ele = queues[queue_num][rq_front[queue_num]];
    rq_front[queue_num] = (rq_front[queue_num] + 1) % QUEUE_LEN;
    rq_size[queue_num]--;
    return ele;
}
//------------------------------------------------

// ------------------ args parser ----------------
void intro();
void parse_process(char *str, int idx);
void parse_args(int argc, char **argv);

// -----------------------------------------------

// ---------------- MLFQ Algorithm ---------------
int current_time = 0;
int completed_processes = 0;
int last_boost_fired = 0;

int min(int a, int b) {
    return a <= b ? a : b;
}

void start_io(int p_idx) {
    int io_idx = processes[p_idx].current_io_burst;
    if (io_idx >= processes[p_idx].io_burst_len || processes[p_idx].io_bursts[io_idx][0] != processes[p_idx].cpu_time_completed)
        return;

    processes[p_idx].status = BLOCKED;
    processes[p_idx].io_complete_time = current_time + processes[p_idx].io_bursts[io_idx][1];

    processes[p_idx].current_io_burst++;
    printf(
        "[P%d][time: %d] - RUNNING -> BLOCKED\n",
        processes[p_idx].pid,
        current_time
    );
}

int handle_priority_boost(int current_time, int final_time) {
    int should_boost = 0;
    for (int i = current_time; i <= final_time; i++) {
        if (i != 0 && i % BOOST_PERIOD == 0 && last_boost_fired != i) {
            should_boost = i;
            last_boost_fired = i;
            break;
        }
    }
    if (!should_boost) return 0;

    printf("[time: %d] - PRIORITY BOOST OF ALL PROCESSESS\n", should_boost);

    // need to move all the process to the top queue
    for (int i = 1; i < QUEUE_NUM; i++) {
        while (!empty(i)) {
            int p_idx = dequeue(i);
            enqueue(0, p_idx);
        }
    }

    // reset their time allottment
    for (int i = rq_front[0]; i != rq_back[0]; i = (i + 1) % QUEUE_LEN) {
        processes[queues[0][i]].time_allottment_rem = TIME_SHARING;
        processes[queues[0][i]].queue_num = 0;
    }
    return 1;
}

void execute_rr_in_queue(int q_idx) {
    int p_idx = dequeue(q_idx);

    processes[p_idx].status = RUNNING;
    printf("[P%d][time: %d] - READY -> RUNNING\n", processes[p_idx].pid, current_time);

    if (processes[p_idx].remaining_time == processes[p_idx].run_time) {
        processes[p_idx].start_time = current_time;
    }

    int time = min(processes[p_idx].time_allottment_rem, processes[p_idx].remaining_time);

    // IO handling
    if (processes[p_idx].current_io_burst < processes[p_idx].io_burst_len) {
        int idx = processes[p_idx].current_io_burst;
        int time_to_io = processes[p_idx].io_bursts[idx][0] - processes[p_idx].cpu_time_completed;
        time = min(time, time_to_io);
    }

    processes[p_idx].remaining_time -= time;
    processes[p_idx].cpu_time_completed += time;
    processes[p_idx].time_allottment_rem -= time;
    int new_current_time = current_time + time;
    
    int boosted = handle_priority_boost(current_time, new_current_time);
    current_time = new_current_time;

    start_io(p_idx);

    // reset time allotement
    if (boosted || processes[p_idx].time_allottment_rem == 0) 
        processes[p_idx].time_allottment_rem = TIME_SHARING;

    // the process went to Blocked state
    if (processes[p_idx].status == BLOCKED) 
        return;

    if (processes[p_idx].remaining_time == 0) {
        processes[p_idx].status = COMPLETED;
        processes[p_idx].end_time = current_time;
        completed_processes++;
        printf("[P%d][time: %d] - RUNNING -> COMPLETED\n", processes[p_idx].pid, current_time);
    } else {
        processes[p_idx].status = READY;
        printf("[P%d][time: %d] - RUNNING -> READY\n", processes[p_idx].pid, current_time);

        int new_q_idx = boosted ? 0: (q_idx == QUEUE_NUM - 1 ? q_idx: q_idx + 1);
        processes[p_idx].queue_num = new_q_idx;
        enqueue(new_q_idx, p_idx);
        
        printf("[P%d][time: %d] - PROCESS DEMOTED FROM Q%d -> Q%d\n", processes[p_idx].pid, current_time, q_idx, new_q_idx);
    }
}

void execute_process() {
    int q_idx = 0;
    while (q_idx < QUEUE_NUM && empty(q_idx))
        q_idx++;

    if (q_idx >= QUEUE_NUM) {
        printf("[time: %d] IDLE CPU\n", current_time);
        handle_priority_boost(current_time, current_time + 1);
        current_time++;
        return;
    }
    execute_rr_in_queue(q_idx);
}

// moves process to NEW -> READY when process arrives
void handle_arrivals() {
    for (int i = 0; i < total_processes; i++) {
        if (processes[i].status == NEW && processes[i].arrival_time <= current_time) {
            // move the process to top priority of queue
            processes[i].status = READY;
            enqueue(0, i);
            printf("[P%d][time: %d] - NEW -> READY\n", processes[i].pid, current_time);
        }
    }
}

void handle_io_completions() {
    for (int i = 0; i < total_processes; i++) {
        if (processes[i].status == BLOCKED && processes[i].io_complete_time <= current_time) {
            // move back this process to ready
            processes[i].status = READY;
            enqueue(processes[i].queue_num, i);
            printf(
                "[P%d][time: %d] - BLOCKED -> READY\n",
                processes[i].pid,
                current_time
            );
        }
    }
}

void mlfq() {
    while (completed_processes < total_processes) {
        handle_arrivals();
        handle_io_completions();
        execute_process();
    }
}
// -----------------------------------------------

// -------------------- summarise ----------------
void sort_process() {
    for (int i = 0; i < total_processes; i++) {
        for (int j = i + 1; j < total_processes; j++) {
            long pi = processes[i].start_time;
            long pj = processes[j].start_time;
            if (pj < pi) {
                proc_info temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void summarise() {
    printf("\n------------------\n");

    printf("\nSummary\n\n");
    sort_process();
    long total_turnaround_time = 0;
    long total_response_time = 0;
    printf("|PID\t| Arrival Time  | Start Time\t| Completion Time | Job Time\t|\n");
    printf("|-------|---------------|---------------|-----------------|-------------|\n");
    for (int i = 0; i < total_processes; i++) {
        printf("|%d\t| %d\t\t| %d\t\t| %d\t\t  | %d\t\t|\n", processes[i].pid, processes[i].arrival_time,
               processes[i].start_time, processes[i].end_time, processes[i].run_time);
        total_turnaround_time += processes[i].end_time - processes[i].arrival_time;
        total_response_time += processes[i].start_time - processes[i].arrival_time;
    }

    printf("|_______|_______________|_______________|_________________|_____________|\n\n");
    printf("Total turnaround time - %ld\n", total_turnaround_time);
    printf("Avarage turnaround time - %.2f\n", ((double)total_turnaround_time) / ((double)total_processes));
    printf("Avarage response time - %.2f\n", ((double)total_response_time) / ((double)total_processes));
    printf("\n------------------\n");

}

// -----------------------------------------------

// main function
int main(int argc, char **argv) {
    parse_args(argc, argv);
    intro();
    mlfq();
    summarise();
    return 0;
}


void negative_check(int num) {
    if (num >= 0)
        return;
    fprintf(stderr, "Please only use integers greater than or equal to 0.");
    exit(1);
}

void invalid_input(int cond) {
    if (cond)
        return;
    fprintf(stderr, "Please use this format for each process: "
                    "arrival_time:job_run_time;io_burst_1_occur_time-io_burst_lasts_for_time,io_burst_2_occur_time-io_"
                    "burst_lasts_for_time\nIf no I/O bursts, then arrival_time:job_run_time;");
    exit(1);
}

void sort_io_bursts(int idx) {
    for (int i = 0; i < processes[idx].io_burst_len; i++) {
        for (int j = i + 1; j < processes[idx].io_burst_len; j++) {
            if (processes[idx].io_bursts[j][0] < processes[idx].io_bursts[i][0]) {
                int tmp0 = processes[idx].io_bursts[j][0];
                int tmp1 = processes[idx].io_bursts[j][1];

                processes[idx].io_bursts[j][0] = processes[idx].io_bursts[i][0];
                processes[idx].io_bursts[j][1] = processes[idx].io_bursts[i][1];

                processes[idx].io_bursts[i][0] = tmp0;
                processes[idx].io_bursts[i][1] = tmp1;
            }
        }
    }
}

// Arg parser implementation
void parse_process(char *str, int idx) {
    int i = 0;
    int num = 0;
    processes[idx].pid = idx + 1;
    while (str[i] != '\0') {
        if (str[i] == ':') {
            negative_check(num);
            processes[idx].arrival_time = num;
            num = 0;
        } else if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
        } else if (str[i] == ';') {
            negative_check(num);
            processes[idx].run_time = num;
            num = 0;
        } else {
            fprintf(stderr, "'%c' is not a valid character, allowed character: digit(0-9), '-', ',', ':', ';'", str[i]);
            exit(1);
        }
        i++;
        if (i - 1 >= 0 && str[i - 1] == ';')
            break;
    }

    invalid_input(i - 1 >= 0 && str[i - 1] == ';');

    processes[idx].io_burst_len = (str[i] != '\0' && (str[i] >= '0' && str[i] <= '9'));
    for (int k = i; str[k] != '\0'; k++) {
        processes[idx].io_burst_len += str[k] == ',';
    }

    processes[idx].io_bursts = calloc(processes[idx].io_burst_len, sizeof(*(processes[idx].io_bursts)));

    int j = 0;

    int flag = 0;

    while (str[i] != '\0') {
        if (str[i] == ',') {
            negative_check(num);
            processes[idx].io_bursts[j][1] = num;
            num = 0;
            flag = 1;
            j++;
        } else if (str[i] == '-') {
            negative_check(num);
            processes[idx].io_bursts[j][0] = num;
            flag = 0;
            num = 0;
        } else if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
        } else {
            fprintf(stderr, "%c is not a valid character, allowed character: digit(0-9), '-', ',', ':', ';'", str[i]);
            exit(1);
        }
        i++;
    }
    if (!flag && num > 0) {
        processes[idx].io_bursts[j][1] = num;
    }
    if (processes[idx].io_burst_len > 1) {
        sort_io_bursts(idx);
    }
    processes[idx].time_allottment_rem = TIME_SHARING;
    processes[idx].remaining_time = processes[idx].run_time;
}

void intro() {
    printf("\n------------------\n");
    for (int i = 0; i < total_processes; i++) {
        printf("P%d: Arrive time %d with run time %d", processes[i].pid, processes[i].arrival_time,
               processes[i].run_time);
        if (processes[i].io_burst_len > 0) {
            printf(", IO Bursts-\n");
            for (int k = 0; k < processes[i].io_burst_len; k++) {
                printf("  %d. At %d for %d time units\n", k + 1, processes[i].io_bursts[k][0],
                       processes[i].io_bursts[k][1]);
            }
        } else {
            printf("\n");
        }
    }
    printf("\n------------------\n");
}

void parse_args(int argc, char **argv) {
    if (argc > 1) {
        if (argc - 1 > 1200) {
            fprintf(stderr, "Upper bound of number of processes is 1200");
            exit(1);
        }
        processes = (proc_info *)calloc(argc - 1, sizeof(proc_info));
        assert(processes != NULL);
        total_processes = argc - 1;
        for (int i = 1; i < argc; i++) {
            parse_process(argv[i], i - 1);
        }
    } else {
        fprintf(stdout, "Usage: ./mlfq process1 process2...\nEach process = "
                        "arrival_time:run_time;io_occur_time-io_lasts_time,io_occur_time-io_lasts_time\n");
        printf("Executing default processes\n");

        processes = (proc_info *)calloc(3, sizeof(proc_info));
        assert(processes != NULL);
        total_processes = 3;
        parse_process("0:100;10-10,50-10", 0);
        parse_process("10:20;", 1);
        parse_process("20:10;", 2);
    }
}