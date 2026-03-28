# include<stdio.h>

// simple queue implementation
typedef struct {
    int threads[100];
    int front;
    int rear;
} queue_t;

void queue_init(queue_t *q) {
    q->front = q->rear = 0;
}

int queue_empty(queue_t *q) {
    return q->front == q->rear;
}

void queue_add(queue_t *q, int tid) {
    q->threads[q->rear++] = tid;
}

int queue_remove(queue_t *q) {
    return q->threads[q->front++];
}

typedef struct __lock_t {
    int guard;
    int flag;
    queue_t *q;
} lock_t;


// note: it's not an atomic
int test_and_set(int *old_ptr, int new) {
    int old = *old_ptr;
    *old_ptr = new;
    return old;
}

// simulate these functions
void park() {
    sleep(1);
}

void unpark(int tid) {
    // wake thread tid
    printf("Thread with id: %d woke up.", tid);
}

void lock_init(lock_t *lock) {
    lock->flag = 0;
    lock->guard = 0;
    queue_init(lock->q);
}

void lock(lock_t *lock) {
    while (test_and_set(&lock->guard, 1) == 1)
        ; // acquire guard lock by spinning

    if (lock->flag == 0) {
        lock->flag = 1; // lock is acquired
        lock->guard = 0;
    } else {
        queue_add(lock->q, gettid());
        lock->guard = 0;
        park(); // go to sleep
    }
}

void unlock(lock_t *lock) {
    while (test_and_set(&lock->guard, 1) == 1)
        ;
    if (queue_empty(lock->q))
        lock->flag = 0; // no one wants the thread, so let it go
    else 
        unpark(queue_remove(lock->q)); // now the sleeping thrad will wake up
    
    // release guard
    lock->guard = 0;
}

