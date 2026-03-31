#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "concurrent-queue.h"
#include "utils.h"

#define OPS_PER_THREAD 100000

queue_t queue;

typedef struct {
    int thread_id;
} thread_arg_t;

void* worker(void* arg) {
    thread_arg_t* t = (thread_arg_t*)arg;

    for (int i = 0; i < OPS_PER_THREAD; i++) {
        queue_enqueue(&queue, i);

        // try dequeue (ignore failure)
        queue_dequeue(&queue);
    }

    return NULL;
}

int main(int argc, char** argv) {
    int num_threads = (argc > 1) ? atoi(argv[1]) : 4;

    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];

    queue_init(&queue);

    double start = get_time();

    // create threads
    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    // join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = get_time();

    double total_ops = (double)num_threads * OPS_PER_THREAD * 2; // enqueue + dequeue
    double time_taken = end - start;

    printf("Threads: %d\n", num_threads);
    printf("Total ops: %.0f\n", total_ops);
    printf("Time: %f seconds\n", time_taken);
    printf("Throughput: %f ops/sec\n", total_ops / time_taken);

    return 0;
}