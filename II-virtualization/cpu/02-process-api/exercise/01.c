#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// I think the child process receives copy of x?
// As child and parent both have initial value of 100
// Yes, I am certain, now that I have created pointer, and both the processes points to the same location
// but it's not changing the value of the either of their process. It means it have created the copy of address space

int main() {
    int *x = malloc(sizeof(int));
    *x = 100;
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("(%d) Child process, x = %d, ptr-x = %p, changing value of x to 50\n", getpid(), *x, x);
        *x = 50;
        printf("(%d) Child process, x = %d, ptr-x = %p\n", getpid(), *x, x);
    } else {
        wait(NULL);
        printf("(%d) Parent process, x = %d, ptr-x = %p, changing value of the x to 120\n", getpid(), *x, x);
        *x = 120;
        printf("(%d) Parent process, x = %d, ptr-x = %p,\n", getpid(), *x, x);
    }
}