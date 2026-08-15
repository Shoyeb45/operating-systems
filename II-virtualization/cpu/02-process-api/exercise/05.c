#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // int rc_wait = wait(NULL);
        printf("(%d) Child process\n", getpid());
        // printf("(%d) rc_wait on child = %d\n", getpid(), rc_wait);
    } else {
        int rc_wait = wait(NULL);
        printf("(%d) Parent process\n", getpid());
        printf("(%d) rc_wait = %d\n", getpid(), rc_wait);
    }
}