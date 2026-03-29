#include<pthread.h>

#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct __node_t {
    int value;
    struct __node_t* next;
} node_t;

typedef struct {
    node_t* head;
    node_t* tail;
    pthread_mutex_t lock;
} list_t;


void list_init(list_t*);
int list_insert(list_t*, int);
int list_search(list_t*, int);
int list_pop(list_t*);
void list_print(list_t*);

#endif