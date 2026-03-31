#include <stdio.h>
#include "concurrent-queue.h"
#include <assert.h>

int main() {
    queue_t* queue;

    queue_init(queue);

    assert(queue->head != NULL);
    assert(queue->head->value == -1);
    assert(queue->head == queue->tail);

    // insert something
    queue_enqueue(queue, 1); // 1
    assert(queue_front(queue) == 1);
    
    queue_enqueue(queue, 2); // 1 2
    assert(queue_front(queue) == 1);
    
    queue_enqueue(queue, 3); // 1 2 3
    assert(queue_dequeue(queue) == 1); // 2 3

    queue_enqueue(queue, 4); // 2 3 4
    assert(queue_dequeue(queue) == 2); // 3 4

    assert(queue_front(queue) == 3);
    assert(queue_dequeue(queue) == 3);
    assert(queue_front(queue) == 4);

}


