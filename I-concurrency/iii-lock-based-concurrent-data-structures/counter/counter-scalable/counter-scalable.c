#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter-scalable.h"

/// @brief Initialise the counter data structure
/// @param counter pointer to the counter_t type
/// @param num_cpus number of cpus to use
/// @param threshold threshold value
int init(counter_t *counter, int num_cpus, int threshold) {
    counter->num_cpus = num_cpus;
    counter->threshold = threshold;
    counter->global = 0;

    counter->local = (int*)calloc(num_cpus, sizeof(int));
    if (counter->local == NULL)
        return -1;

    counter->local_lock = (pthread_mutex_t*)calloc(num_cpus, sizeof(pthread_mutex_t));
    if (counter->local_lock == NULL)
        return -1;

    int rc = pthread_mutex_init(&counter->global_lock, NULL);
    if (rc != 0) return -1;

    for (int i = 0; i < num_cpus; i++) {
        counter->local[i] = 0;
        int rc = pthread_mutex_init(&counter->local_lock[i], NULL);
        if (rc != 0) return -1;
    }

    return 1;
}


void update(counter_t* counter, int thread_id, int amount) {
    int cpu = thread_id % counter->num_cpus;

    pthread_mutex_lock(&counter->local_lock[cpu]);
    
    counter->local[cpu] += amount;
    // update if it exceeds threshold
    if (counter->local[cpu] >= counter->threshold) {
        pthread_mutex_lock(&counter->global_lock);
        counter->global += counter->local[cpu];
        pthread_mutex_unlock(&counter->global_lock);
        counter->local[cpu] = 0;
    }
    pthread_mutex_unlock(&counter->local_lock[cpu]);
}

int get(counter_t* counter) {
    pthread_mutex_lock(&counter->global_lock);
    int val = counter->global;
    pthread_mutex_unlock(&counter->global_lock);
    return val;
}
