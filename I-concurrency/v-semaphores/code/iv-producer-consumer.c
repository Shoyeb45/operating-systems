#include <semaphore.h>
#include <stdio.h>

sem_t full, empty;
sem_t mutex;
int loops = 12;
#define MAX 10
int buffer[MAX];

int fill = 0;
int use = 0;

void put(int val) {
    buffer[fill] = val;
    fill = (fill + 1) % MAX;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void* producer(void* args) {
    for (int i = 0; i < loops; i++) {
        // first acquire the empty semaphore and then acquire the lock for avoiding the lock contention
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);

        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consumer(void *args) {
    for (int i = 0; i < loops; i++) {
        sem_wait(&full);
        sem_wait(&mutex);

        int ele = get();
        
        sem_post(&mutex);
        sem_post(&empty);
        printf("Consumer: %d", ele);
    }
    return NULL;
}
