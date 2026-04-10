#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define LOOPS 12
#define MAX 10

pthread_cond_t fill, empty;
pthread_mutex_t mutex;

volatile int buffer[MAX];
volatile int count = 0;
int fill_ptr = 0;
int use_ptr = 0;

void init() {
    pthread_cond_init(&fill, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_mutex_init(&mutex, NULL);
}

void put(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
    count++;
} 

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count--;
    return tmp;
}
void* producer(void* args) {
    usleep(100000);
    int i;
    for (i = 0; i < LOOPS; i++) {
        // acquire the lock
        pthread_mutex_lock(&mutex);
        // if the buffer is full, then just wait

        while (count == MAX) {
            printf("Producer going to sleep\n");
            pthread_cond_wait(&empty, &mutex);
        }
        // insert element into the buffer
        put(i);
        // tell the consumer that buffer has been filled
        pthread_cond_signal(&fill);
        printf("Consumer woke up\n");
        pthread_mutex_unlock(&mutex);
        printf("Produced: %d\n", i);
    }
    return NULL;
}

void* consumer(void* args) {
    usleep(100000);
    int i;
    for (i = 0; i < LOOPS; i++) {
        // get the lock
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            printf("Consumer going to sleep\n");
            pthread_cond_wait(&fill, &mutex);
        }
        
        int ele = get();
        pthread_cond_signal(&empty);
        printf("Producer woke up\n");
        pthread_mutex_unlock(&mutex);
        printf("Consumed: %d\n", ele);
    }
    return NULL;
}

int main() {
    init();
    pthread_t c, p;

    pthread_create(&c, NULL, consumer, NULL);
    pthread_create(&p, NULL, producer, NULL);

    pthread_join(c, NULL);
    pthread_join(p, NULL);
}