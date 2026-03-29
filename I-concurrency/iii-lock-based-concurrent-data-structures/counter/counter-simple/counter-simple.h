# include<pthread.h>

# ifndef COUNTER_H
# define COUNTER_H

typedef struct {
    long value;
    pthread_mutex_t lock;    
} counter_t;

void init(counter_t*);
void increment(counter_t*);
void decrement(counter_t*);
int get(counter_t*);

# endif