#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void* child(void* args) {
    printf("Child\n");
    sem_post(&sem);
    return NULL;
}

int main() {
    sem_init(&sem, 0, 0);
    printf("Parent begin\n");
    
    pthread_t th;
    pthread_create(&th, NULL, child, NULL);

    sem_wait(&sem); // wait here for the child
    printf("Parent end\n");
}