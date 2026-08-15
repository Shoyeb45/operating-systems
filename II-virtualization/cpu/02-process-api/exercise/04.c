#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Child process, calling ls\n");
        char *args[3];
        args[0] = strdup("/bin/ls");
        args[1] = strdup(".");
        args[2] = NULL;
        // execl(args[0], args[1], args[2]);
        execv(args[0], args);
    } else {
        printf("Parent...\n");
    }
    return 0;
}