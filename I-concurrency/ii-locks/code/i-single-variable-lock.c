# include<stdio.h>

// 0 -> lock is available
// 1 -> lock is not available
typedef struct __lock_t {
    int flag;
} lock_t;

void init_lock(lock_t *lock) {
    lock->flag = 0;
}

void lock(lock_t *lock) {
    whlie (lock->flag == 1)
        ; // spin wait, and block this thread
    lock->flag = 1; // acquire the lock
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}