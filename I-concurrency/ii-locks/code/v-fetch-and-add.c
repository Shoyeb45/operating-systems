# include<stdio.h>


typedef struct __lock_t {
    int turn;
    int ticket;
} lock_t;


void lock_init(lock_t *lock) {
    lock->ticket = 0;
    lock->turn = 0;
}

void unlock(lock_t *lock) {
    lock->turn = lock->turn + 1;
}

void lock(lock_t *lock) {
    int my_turn = fetch_and_add(&lock->ticket);
    while (lock->turn != my_turn) 
        ; // spin
}
int fetch_and_add(int *ptr) {
    int old = *ptr;
    *ptr = old + 1;
    return old;
}