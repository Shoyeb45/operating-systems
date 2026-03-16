# include<stdio.h>
# include<stdatomic.h>


// lock
// 0: lock is avilable
// 1: lock is held
typedef struct __lock_t {
    atomic_int flag;
} lock_t;

void init(lock_t *lock) {
    // lock->flag = 0;
    atomic_store(&lock->flag, 0);
}

void lock(lock_t *lock) {
    while (atomic_exchange(&lock->flag, 1) == 1) 
        ;
}

void unlock(lock_t *lock) {
    // lock->flag = 0;
    atomic_store(&lock->flag, 0);
}

// Below is not real atomic function, it's just for understanding how atomic_exchange works under the cpu instruction
int test_and_set(int *old_ptr, int new) {
    int old = *old_ptr;
    *old_ptr = new;
    return old;
}

int main() {
    int l = 1;
    printf("Old: %d\n", test_and_set(&l, 0));
    printf("new: %d\n", l);
}