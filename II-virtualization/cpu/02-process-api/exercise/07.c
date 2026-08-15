#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("(%d) Child process\n", getpid());
        printf("(%d) Closing stdout\n", getpid());
        close(STDOUT_FILENO);
        printf("(%d) Closed stdout\n", getpid());
    } else {
        wait(NULL);
        printf("(%d) Parent process\n", getpid());
    }
}