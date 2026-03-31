#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "scalable-linked-list.h"

#define INITIAL_SIZE 100000
#define SEARCH_PERCENT 90

typedef struct {
    list_t* list;
    int operations;
    int thread_id;
} thread_arg_t;

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void* worker(void* arg) {
    thread_arg_t* t = (thread_arg_t*)arg;

    // thread-local seed (important for randomness in multithreading)
    unsigned int seed = time(NULL) ^ t->thread_id;

    for (int i = 0; i < t->operations; i++) {
        int r = rand_r(&seed) % 100;

        if (r < SEARCH_PERCENT) {
            int key = rand_r(&seed) % INITIAL_SIZE;
            list_search(t->list, key);
        } else {
            int val = rand_r(&seed);
            list_insert(t->list, val);
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    int num_threads = argc > 1 ? atoi(argv[1]) : 8;
    int ops_per_thread = 100000;

    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];

    list_t list;
    list_init(&list);

    // 🔥 Step 1: Pre-fill list
    for (int i = 0; i < INITIAL_SIZE; i++) {
        list_insert(&list, i);
    }

    printf("Pre-filled list with %d elements\n", INITIAL_SIZE);

    double start = get_time();

    // 🔥 Step 2: Run threads
    for (int i = 0; i < num_threads; i++) {
        args[i].list = &list;
        args[i].operations = ops_per_thread;
        args[i].thread_id = i;

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