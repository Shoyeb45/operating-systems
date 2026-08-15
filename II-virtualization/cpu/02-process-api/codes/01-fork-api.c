#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Hello World (pid: %d)\n", getpid());

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello, I am child process (pid: %d)\n", getpid());
    } else {
        printf("Hello, I am parent of %d (pid: %d)\n", rc, getpid());
    }
    return 0;
}