#include<stdio.h>
#include<linux/futex.h>
#include <sys/syscall.h>
#include <stdatomic.h>
#include <unistd.h>

// 0: unlocked
// 1: locked
typedef struct {
    int val;
} futex_lock_t;

int futex_wait(int *addr, int val) {
    return syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL, NULL, 0);
}

int futex_wake(int *addr, int n) {
    return syscall(SYS_futex, addr, FUTEX_WAKE, n, NULL, NULL, 0);
}

void lock(futex_lock_t *lock) {
    int expected = 0;

    // Fast path: try to acquire lock
    if (atomic_compare_exchange_strong(&lock->val, &expected, 1)) {
        return;
    }

    // Slow path
    while (1) {
        expected = 0;

        // Try again
        if (atomic_compare_exchange_strong(&lock->val, &expected, 1)) {
            return;
        }

        // Sleep if still locked
        futex_wait(&lock->val, 1);
    }
}

void unlock(futex_lock_t *lock) {
    atomic_store(&lock->val, 0);

    // Wake one waiting thread
    futex_wake(&lock->val, 1);
}