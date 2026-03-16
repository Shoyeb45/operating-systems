#include <stdio.h>
# include<stdatomic.h>

typedef struct __lock_t {
    atomic_int flag;
} lock_t;



// here is the pseudo code
int load_linked(int *ptr) {
    return *ptr;
}

int store_conditional(int *ptr, int value) {
    // if (no update to * ptr since LoadLinked to this address) {
    //     *ptr = value;
    //     return 1; // success!
    // } else {
    //     return 0; // failed to update
    // }
}

void lock(lock_t *lock) {
    int expected = 0;
    while (1) {
        while (load_linked(&lock->flag) == 1) 
            ; 
        
        if (store_conditional(&lock->flag, 1) == 1) return;

        // the real implementation is:
        // if (atomic_compare_exchange_weak(&lock->flag, &expected, 1)) return;
    }
}

void unlock(lock_t *lock) {
    atomic_store(&lock->flag, 0);
}

void init(lock_t *lock) {
    atomic_store(&lock->flag, 0);
}