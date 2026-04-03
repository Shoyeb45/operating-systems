#include <stdio.h>
#include <pthread.h>

// state variable to hold the 
volatile int done = 1;

void* child(void* args) {
    printf("Child Thread\n");
    done = 0;
    return NULL;
}

void thread_join() {
    while (done == 1) {
        // spin lock
    };
}

int main() {
    printf("Main Thread\n");
    pthread_t p;

    pthread_create(&p, NULL, child, NULL);

    // wait for thread to complete
    thread_join();

    printf("Parent done\n");
}