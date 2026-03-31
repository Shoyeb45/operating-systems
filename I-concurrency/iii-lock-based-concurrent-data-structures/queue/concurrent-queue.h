#include <pthread.h>

#ifndef CONCURRENT_QUEUE
#define CONCURRENT_QUEUE

typedef struct __node_t {
    int value;
    struct __node_t* next;
} node_t;

typedef struct __queue_t {
    node_t *head;
    node_t *tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

int queue_init(queue_t*);
int queue_enqueue(queue_t*, int);
int queue_dequeue(queue_t*);
int queue_front(queue_t*);

#endif