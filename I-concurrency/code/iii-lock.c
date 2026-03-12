# include<pthread.h>
# include<stdio.h>
# include<stdlib.h>
# include<assert.h>

pthread_mutex_t lock;

void init_lock(pthread_mutex_t *lock) {
    int err = pthread_mutex_init(lock, NULL);
    assert(err == 0);
}

static volatile int cntr = 0;

void* perform_task(void *arg) {
    printf("Executing thread %s\n", (char*)arg);

    for (int i = 0; i < 1000; i++) {
        // mutex lock and critical section
        pthread_mutex_lock(&lock);    
        cntr = cntr + 1;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
} 

int main() {
    init_lock(&lock);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, perform_task, (void*)"A");
    pthread_create(&t2, NULL, perform_task, (void*)"B");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Done with execution of threads, %d", cntr);
    pthread_mutex_destroy(&lock);
    return 0;
}
