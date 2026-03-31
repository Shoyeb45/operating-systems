#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "concurrent-queue.h"

node_t* create_node(int value) {
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("Failed to create new node");     
        return NULL;   
    }
    new_node->value = value;
    new_node->next = NULL;  

    return new_node;
}


int queue_init(queue_t *queue) {
    node_t* dummy = create_node(-1);
    if (dummy == NULL) {
        return -1;
    }
    dummy->next = NULL;
    queue->head = queue->tail = dummy;

    int rc = pthread_mutex_init(&queue->head_lock, NULL);
    if (rc != 0) return -1;
    rc = pthread_mutex_init(&queue->tail_lock, NULL);
    if (rc != 0) return -1;

    return 0;
}

int queue_enqueue(queue_t *queue, int value) {
    node_t *new_node = create_node(value);
    if (new_node == NULL) return -1;

    // as we have to insert the new value
    // we will acquire tail lock
    pthread_mutex_lock(&queue->tail_lock);
    queue->tail->next = new_node;
    queue->tail = new_node;
    pthread_mutex_unlock(&queue->tail_lock);

    return 0;
}

int queue_dequeue(queue_t *queue) {
    // acquire head lock
    pthread_mutex_lock(&queue->head_lock);

    node_t *old_head = queue->head;
    node_t *new_head = old_head->next;
    
    if (new_head == NULL) {
        // empty queue
        printf("Queue is empty.");
        return -1;
    }
    int value = new_head->value;
    queue->head = new_head;
    pthread_mutex_unlock(&queue->head_lock);
    
    free(old_head);
    return value;
}

int queue_front(queue_t *queue) {
    pthread_mutex_lock(&queue->head_lock);
    
    node_t *front = queue->head->next;
    if (front == NULL) {
        printf("Queue is empty.");
        return -1;
    }
    int value = front->value;
    pthread_mutex_unlock(&queue->head_lock);

    return value;
}