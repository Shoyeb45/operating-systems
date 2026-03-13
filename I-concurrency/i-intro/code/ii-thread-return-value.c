# include<pthread.h>
# include<stdio.h>
# include<stdlib.h>

typedef struct {
    int a;
    int b;
} myarg_t;

typedef struct {
    int x;
    int y;
} myret_t;

void* task(void* arg) {
    int a = ((myarg_t*)arg)->a;
    int b = ((myarg_t*)arg)->b;

    printf("%d and %d\n", a, b);

    // allocate memory in heap
    myret_t *ret = (myret_t*) malloc(sizeof(myret_t));
    ret->x = 12;
    ret->y = 14;
    return (void*) ret;
}

int main() {
    pthread_t p;

    myarg_t arg = { 12, 4 };
    pthread_create(&p, NULL, task, &arg);

    myret_t *return_value;
    pthread_join(p, (void**) &return_value);

    printf("Thread returned: %d and %d", return_value->x, return_value->y);
    free(return_value);
    return 0;
}