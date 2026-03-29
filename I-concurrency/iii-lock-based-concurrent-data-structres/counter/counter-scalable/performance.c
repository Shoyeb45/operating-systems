#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "counter-scalable.h"

#define OPS_PER_THREAD 1000000

long get_cpus() {
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 1) {
        // Fallback or error handling
        nprocs = 1;
    }
    return nprocs;
}

counter_t counter;

// helper to get time
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// this common function will be executed by our threads
void* worker(void *arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        update(&counter, thread_id, 1);
    }
    return NULL;
}

int main(int argc, char** argv) {
    // int num_threads = NUM_THREADS;
    int num_threads = get_cpus();
    if (argc > 1 && atoi(argv[1]) != 0) {
        num_threads = atoi(argv[1]);
    }
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    init(&counter, num_threads, 1000); // threshold

    double start_time = get_time();

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end_time = get_time();

    printf("Total threads used %d\n", num_threads);
    printf("Final counter: %d\n", get(&counter));
    printf("Time taken: %f seconds\n", end_time - start_time);

    double total_ops = (double)num_threads * OPS_PER_THREAD;
    printf("Throughput: %.2f ops/sec\n", total_ops / (end_time - start_time));
}