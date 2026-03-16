# include<stdio.h>
#include <stdatomic.h>

typedef struct __lock_t {
    atomic_int flag;
} lock_t;

// This one will only assign the variable when expected one matches with the value
// compare_and_swap(&lock, 0, 1)
// The it will assign lock to 1, only if the *lock is 0
int compare_and_swap(atomic_int *ptr, int expected, int new) {
    int original = *ptr;
    if (original == expected) {
        *ptr = new;
    }
    return original;
}

void init(lock_t *lock) {
    atomic_store(&lock->flag, 0);
}

void lock(lock_t *lock) {
    int expected = 0;
    while (atomic_compare_exchange_strong(&lock->flag, &expected, 1) == 1) 
        ;
}

void unlock(lock_t *lock) {
    atomic_store(&lock->flag, 0);
}