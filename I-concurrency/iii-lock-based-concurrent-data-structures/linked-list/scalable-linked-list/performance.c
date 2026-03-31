#include <stdio.h>
#include "scalable-linked-list.h"
#include <time.h>
#include <stdlib.h>

typedef struct {
    list_t* list;
    int operations;
} thread_arg_t;

void* worker(void* arg) {
    thread_arg_t* t = (thread_arg_t*)arg;

    for (int i = 0; i < t->operations; i++) {
        list_insert(t->list, i);
    }

    return NULL;
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char** argv) {
    int num_threads = argc > 1 && atoi(argv[1]) != 0 ? atoi(argv[1]) : 8;
    int ops_per_thread = 100000;

    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];

    list_t list;
    list_init(&list);

    double start = get_time();

    for (int i = 0; i < num_threads; i++) {
        args[i].list = &list;
        args[i].operations = ops_per_thread;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = get_time();

    int total_ops = num_threads * ops_per_thread;

    printf("Threads: %d\n", num_threads);
    printf("Total ops: %d\n", total_ops);
    printf("Time: %f seconds\n", end - start);
    printf("Throughput: %f ops/sec\n", total_ops / (end - start));

    return 0;
}
