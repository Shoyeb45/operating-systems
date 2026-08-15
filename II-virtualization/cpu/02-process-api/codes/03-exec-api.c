#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    printf("Hello, World (pid: %d)\n", getpid());

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello, I am child (pid: %d)\n", getpid());
        char *args[4];
        args[0] = strdup("grep");
        args[1] = strdup("args");
        args[2] = strdup("exec-api.c");
        args[3] = NULL;
        execvp(args[0], args);
        printf("This shouldn't print out\n");
    } else {
        int rc_wait = wait(NULL);
        printf("Hello, I am parent of %d (rc_wait: %d) (pid: %d)", rc, rc_wait, getpid());
    }
    return 0;
}