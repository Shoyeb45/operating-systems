#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

sem_t sem;

int counter = 0;

void* worker(void *args) {
    int id = *(int*)args;

    sem_wait(&sem);

    int temp = counter;
    temp++;
    counter = temp;

    printf("Thread %d, counter = %d\n", id, counter);

    sem_post(&sem);
    return NULL;
}

int main() {
    sem_init(&sem, 0, 1);

    pthread_t th1, th2;

    int id1 = 1, id2 = 2;
    pthread_create(&th1, NULL, worker, &id1);
    pthread_create(&th2, NULL, worker, &id2);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

}