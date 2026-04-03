#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t conditional;
// state variable
volatile int done = 0;

void init() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditional, NULL);
}

void* child(void *args) {
    printf("Child thread start\n");
    int k = 0;
    for (int i = 0; i < (int)1e8; i++) {
        k = i;
    }
    printf("Child thread end\n");

    // acquire the lock and update the state
    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_cond_signal(&conditional);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void thread_join() {
    // acquire the lock
    pthread_mutex_lock(&mutex);
    while (done == 0)
        pthread_cond_wait(&conditional, &mutex);
    
    pthread_mutex_unlock(&mutex);
}

int main() {
    init();

    printf("Parent Thread\n");
    
    pthread_t p;
    pthread_create(&p, NULL, child, NULL);
    
    thread_join();
    printf("Parent Finished\n");
}