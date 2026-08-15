#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main() {
    pid_t rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("(%d) Child process\n", getpid());
    } else {
        int rc_wait = waitpid(rc, NULL, NULL);
        assert(rc_wait == rc);
        printf("(%d) Parent process\n", getpid());
        printf("(%d) rc_wait = %d\n", getpid(), rc_wait);
    }
}