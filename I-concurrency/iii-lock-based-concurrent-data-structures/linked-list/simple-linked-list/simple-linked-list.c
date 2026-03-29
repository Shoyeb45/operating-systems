#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "simple-linked-list.h"

void list_init(list_t* list) {
    list->head = NULL;
    list->tail = NULL;
    pthread_mutex_init(&list->lock, NULL);
}

int list_insert(list_t* list, int value) {
    // create new node without synchronization
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("Failed to insert.\n");
        return -1;
    }
    new_node->value = value;
    new_node->next = NULL;
    
    // now acquire lock
    pthread_mutex_lock(&list->lock);
    if (!list->head) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}

int list_search(list_t* list, int value) {
    int rv = -1;

    pthread_mutex_lock(&list->lock);

    node_t* curr = list->head;
    while (curr) {
        if (curr->value == value) {
            rv = 0;
            break;
        }
        curr = curr->next;
    }

    pthread_mutex_unlock(&list->lock);
    return rv;
}

int list_pop(list_t* list) {
    int rv = -1;
    
    pthread_mutex_lock(&list->lock);
    if (list->tail && list->tail == list->head) {
        node_t* node = list->tail;
        list->tail = list->head = NULL;
        free(node);
        rv = 0;
    } else if (list->tail) {
        node_t* node = list->tail;

        node_t* curr = list->head;
        while (curr && curr->next) {
            if (curr->next == node) {
                break;
            }
            curr = curr->next;
        }

        if (curr) {
            curr->next = NULL;
            list->tail = curr;
            free(node);
        }
    }

    pthread_mutex_unlock(&list->lock);
    return rv;
}

void list_print(list_t* list) {
    pthread_mutex_lock(&list->lock);
    node_t* curr = list->head;
    
    while (curr) {
        printf("%d ", curr->value);
        curr = curr->next;
    }
    printf("\n");
    pthread_mutex_unlock(&list->lock);
}