#include <semaphore.h>

typedef struct __rwlock_t {
    sem_t lock; // binary sem, just for basic locking
    sem_t writelock; // allow one writer and many readers
    int readers; // how many readers are there in critical section
} rwlock_t;

void rwlock_init(rwlock_t* rw) {
    rw->readers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t* rw) {
    sem_wait(rw->lock);
    rw->readers++;
    // for first reader, give them write lock
    if (rw->readers == 1) 
        sem_wait(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t* rw) {
    sem_wait(&rw->lock);
    rw->readers--;

    // we have to release the write lock
    if (rw->readers == 0) 
        sem_post(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t* rw) {
    sem_wait(rw->writelock);
}

void rwlock_release_writelock(rwlock_t* rw) {
    sem_post(&rw->writelock);
}