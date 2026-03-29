#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "counter-simple.h"

// we will spawn 8 threads and increment the counter 1000000 times
#define NUM_THREADS 8
#define OPS_PER_THREAD 1000000

counter_t counter;

// helper to get time
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// this common function will be executed by our threads
void* worker(void *args) {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        increment(&counter);
    }
    return NULL;
}

int main(int argc, char** argv) {
    int num_threads = NUM_THREADS;
    if (argc > 1 && atoi(argv[1]) != 0) {
        num_threads = atoi(argv[1]);
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

    // initialise the counter
    init(&counter);

    // start time
    double start_time = get_time();

    // create each thread
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    // join the threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // get end time
    double end_time = get_time();
    printf("Total threads used %d\n", num_threads);
    printf("Final counter: %ld\n", counter.value);
    printf("Time taken: %f seconds\n", end_time - start_time);

    double total_ops = (double)num_threads * OPS_PER_THREAD;
    printf("Throughput: %.2f ops/sec\n", total_ops / (end_time - start_time));
}