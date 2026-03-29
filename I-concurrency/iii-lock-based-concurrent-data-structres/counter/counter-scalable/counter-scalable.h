# include<pthread.h>

# ifndef COUNTER_H_S
# define COUNTER_H_S

typedef struct {
    int global;
    pthread_mutex_t global_lock;
    int* local;
    pthread_mutex_t* local_lock;
    int threshold;
    int num_cpus;
} counter_t;

int init(counter_t*, int, int);
void update(counter_t*, int, int);
int get(counter_t*);

# endif