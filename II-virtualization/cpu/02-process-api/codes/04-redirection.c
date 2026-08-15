#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int rc = fork();
    
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        close(STDOUT_FILENO);
        open("p4.output", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);

        char *args[3];
        args[0] = strdup("wc");
        args[1] = strdup("04-redirection.c");
        args[2] = NULL;
        execvp(args[0], args);
    } else {
        int rc_wait = wait(NULL);
    }

    return 0;
}