#include <semaphore.h>

sem_t sem;

void init() {
    // This will set initial value, the second argument tell that this semaphore will be used in the same process
    sem_init(&sem, 0, 1);
}
int sem_wait(sem_t *s) {
    // decrement the value of semaphore s by one
    // wait if value of semaphore s is negative
}

int sem_post(sem_t *s) {
    // increment the value of semaphore s by one
    // if there are one or more threads waiting, wake one
}
