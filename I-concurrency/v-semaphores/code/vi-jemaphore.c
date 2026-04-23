#include <pthread.h>
#include <stdio.h>

typedef struct __Jem_t {
    int value;
    pthread_cond_t cond;
    pthread_mutex_t lock;
} Jem_t;

void jem_init(Jem_t *jem, int value) {
    jem->value = value;
    pthread_cond_init(&jem->cond, NULL);
    pthread_mutex_init(&jem->lock, NULL);
}

void jem_wait(Jem_t *jem) {
    pthread_mutex_lock(&jem->lock);

    while (jem->value <= 0)
        pthread_cond_wait(&jem->cond, &jem->lock);
    jem->value--;
    pthread_mutex_unlock(&jem->lock);
}

void jem_post(Jem_t *jem) {
    pthread_mutex_lock(&jem->lock);

    jem->value++;
    pthread_cond_signal(&jem->cond);
    pthread_mutex_unlock(&jem->lock);
}