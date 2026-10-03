#include <stdio.h>
#include <stdlib.h>

int main() {
    void *mem = malloc(12);
    
    *((int*)mem) = 12;    
    *((double*)((int*) mem + 1)) = 43.2;
    
    printf("Address of int: %p\n", mem);
    printf("Address of double: %p\n", (char*) mem + sizeof(int));

    printf("%lf\n", *((double*)((int*) mem + 1)));
    return 0;
}