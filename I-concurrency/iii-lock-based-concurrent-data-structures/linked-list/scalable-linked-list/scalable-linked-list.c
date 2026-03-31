#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scalable-linked-list.h"


// helper function:
node_t* create_node(int value) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    
    node->value = value;
    node->next = NULL;
    pthread_mutex_init(&node->lock, NULL);

    return node;
}


void list_init(list_t* list) {
    list->head = NULL;
    list->tail = NULL;
    pthread_mutex_init(&list->lock, NULL);
}

int list_insert(list_t* list, int value) {
    // create new node without synchronization
    node_t* new_node = create_node(value);
    
    // now acquire lock
    pthread_mutex_lock(&list->lock);
    node_t* old_tail = list->tail;
    if (!old_tail) {
        list->head = list->tail = new_node;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }

    pthread_mutex_unlock(&list->lock);

    // now acquire tail lock
    pthread_mutex_lock(&old_tail->lock);
    
    old_tail->next = new_node;
    pthread_mutex_lock(&list->lock);
    list->tail = new_node;
    pthread_mutex_unlock(&list->lock);

    pthread_mutex_unlock(&old_tail->lock);
    return 0;
}

int list_search(list_t* list, int value) {
    int rv = -1;

    node_t* curr;
    pthread_mutex_lock(&list->lock);
    curr = list->head;
    // now acquire the curr node's lock
    if (curr)
        pthread_mutex_lock(&curr->lock);

    // now release global lock for the list
    pthread_mutex_unlock(&list->lock);

    while (curr) {
        if (curr->value == value) {
            rv = 0;
            // we need to the current node's lock from here, else it will never be release
            pthread_mutex_unlock(&curr->lock); 
            break;
        }
        pthread_mutex_t* curr_node_lock = &curr->lock;

        curr = curr->next;
        // acquire the next node lock
        if (curr) {
            pthread_mutex_lock(&curr->lock);
        }
        // release the current node
        pthread_mutex_unlock(curr_node_lock);
    }

    return rv;
}

int list_pop(list_t* list) {
    pthread_mutex_lock(&list->lock);
    node_t* head = list->head;
    // list is empty
    if (!head) {
        pthread_mutex_unlock(&list->lock);
        return -1;
    }

    // single node case
    if (list->tail == head) {
        // acquire head lock
        pthread_mutex_lock(&head->lock);
        list->head = list->tail = NULL;
        pthread_mutex_unlock(&list->lock);

        // unlock head lock
        pthread_mutex_unlock(&head->lock);
        // destroy head lock
        pthread_mutex_destroy(&head->lock);
        free(head);
        return 0;
    }


    // more than one node
    node_t* prev = head;
    pthread_mutex_lock(&prev->lock);
    node_t* curr = prev->next;
    if (curr)
        pthread_mutex_lock(&curr->lock);
    
    pthread_mutex_unlock(&list->lock);
    
    while (curr->next) {
        node_t* next = curr->next;
        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&prev->lock);
        prev = curr;
        curr = next;
    }

    // now prev points to last second node, and curr points to tail
    prev->next = NULL;
    pthread_mutex_lock(&list->lock);
    list->tail = prev;
    pthread_mutex_unlock(&list->lock);

    // destroy tail
    pthread_mutex_unlock(&curr->lock);
    pthread_mutex_destroy(&curr->lock);
    free(curr);
    
    // unlock the prev
    pthread_mutex_unlock(&prev->lock);

    return 0;
}

void list_print(list_t* list) {
    
    pthread_mutex_lock(&list->lock);
    node_t* curr = list->head;
    if (curr) 
        pthread_mutex_lock(&curr->lock);
    pthread_mutex_unlock(&list->lock);
    
    while (curr) {
        printf("%d ", curr->value);
        pthread_mutex_t* curr_node_lock = &curr->lock;
        curr = curr->next;
        if (curr) 
            pthread_mutex_lock(&curr->lock);

        pthread_mutex_unlock(curr_node_lock);
        
    }
    printf("\n");
}