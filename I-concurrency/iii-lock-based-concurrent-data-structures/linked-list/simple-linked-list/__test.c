#include <stdio.h>
#include "simple-linked-list.h"
#include <assert.h>


int main() {
    list_t* list;
    list_init(list);

    assert(list->head == NULL);
    
    list_insert(list, 12);
    assert(list->head->value == 12);
    assert(list->tail->value == 12);
    list_print(list);
    
    list_insert(list, 24);
    list_print(list);
    assert(list->head->value == 12);
    assert(list->tail->value == 24);
    
    list_insert(list, 1);
    list_insert(list, -10);
    list_print(list);
    assert(list->tail->value == -10);
    
    assert(list_search(list, -10) == 0);
    
    // remove the node
    list_pop(list);
    list_print(list);
    assert(list->tail->value == 1);
    
    list_pop(list);
    assert(list->head->value == 12);
    assert(list->tail->value == 24);
    list_print(list);
}