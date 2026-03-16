# include<stdio.h>
# include<sched.h>

typedef struct __lock_t {
    int flag;
} lock_t;


void lock_init(lock_t *lock) {
    lock->flag = 0;
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}

void yield() {
    // give up the thread voluntarily
    sched_yield();
}

void lock(lock_t *lock) {
    while (test_and_set(&lock->flag, 1) == 1) 
        yield(); 
}

// This is just for understanding purpose, the real implementation will use cpu instructions and atomic variables
int test_and_set(int *ptr, int new) {
    int old = *ptr;
    *ptr = new;
    return old;
}