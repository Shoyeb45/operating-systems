# Semaphores

- A semaphores is an object with integer value which can be used as locks as well as conditional variables.
- See how can be semaphore initialized in c: [semaphores](./code/i-semaphore-routines.c)

## Binary Semaphore(Locks)

- We can turn our semaphore into the lock. 
- Initialise semaphore with `1`, and whenever we are entering critical section, call `sem_wait`, so whenever thread will try to acquire the lock, it will execute this function, and if the value is less than 0, then it will put itself to sleep.
- For releasing the lock, call `sem_post`, this will increment the value and wake up the waiting thread.
- Since lock have binary state, 1(not held) and 0(held), thus we can use semaphore as a locks.
- But one should be vary, when should we use semaphore as lock. This doesn't have lock ownership, so it may not be safe in every case for critical section.

## 