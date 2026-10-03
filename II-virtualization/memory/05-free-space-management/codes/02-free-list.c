#include <stdio.h>
#include <sys/mman.h>

typedef struct __node_t {
    int size;
    struct __node_t *next; 
} node_t;

const int TOTAL_FREE_SPACE = 4096;

int main() {
    node_t *head = mmap(NULL, TOTAL_FREE_SPACE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = TOTAL_FREE_SPACE - sizeof(node_t);
    head->next = NULL;

    printf("%p\n", head);
    printf("%d\n", head->size);
}