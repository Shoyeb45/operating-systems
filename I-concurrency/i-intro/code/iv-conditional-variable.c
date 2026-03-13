# include<stdio.h>
# include<pthread.h>
# include<unistd.h>

# define BUFFER_SIZE 5

// shared buffer between threads
int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t lock;
pthread_cond_t not_full, not_empty;

void* producer(void* args) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&lock);
        // critical section

        while (count == BUFFER_SIZE) {
            // producer has to wait for the consumer to consume the buffer 
            printf("Producer waiting, buffer full\n");
            pthread_cond_wait(&not_full, &lock);
        }

        // produce item
        buffer[count++] = i;
        printf("Produced %d\n", i);

        // tell consumer to consume the buffer
        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* args) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&lock);

        while (count == 0) {
            // nothing to consume, so wait for producer to produce something
            printf("Consumer waiting, buffer empty\n");
            pthread_cond_wait(&not_empty, &lock);
        }

        // consume item
        int item = buffer[--count];
        printf("Consumed %d\n", item);

        // give signal that item is consumed
        pthread_cond_signal(&not_full);

        // release the lock
        pthread_mutex_unlock(&lock);
        sleep(4);
    }
    return NULL;
}

int main() {
    // initialize values
    pthread_t prod, cons;
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_mutex_init(&lock, NULL);

    // producer
    pthread_create(&prod, NULL, producer, NULL);
    // consumer
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
}