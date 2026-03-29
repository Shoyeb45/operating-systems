#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter-simple.h"

void init(counter_t *c) {
    c->value = 0;
    int rc = pthread_mutex_init(&c->lock, NULL);
    if (rc != 0) {
        printf("Failed to initialise mutex lock");
        exit(1);
    }
}

void increment(counter_t *c) {
    // acquire the lock
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);

    return rc;
}

