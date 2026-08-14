#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int *p = malloc(sizeof(int));
    assert(p != NULL);

    printf("(%d) memory address of p: %p\n", getpid(), p);

    *p = 0;
    while (1) {
        sleep(1);
        *p = *p + 1;
        printf("(%d) p: %d\n", getpid(), *p);
    }
    return 0;
}