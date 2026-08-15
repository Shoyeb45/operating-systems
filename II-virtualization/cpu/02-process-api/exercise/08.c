#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pip[2];
    int res_pipe = pipe(pip);
    char str[100];

    if (res_pipe == -1) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    int rc1 = fork();
    if (rc1 < 0) {
        fprintf(stderr, "fork1 failed\n");
        exit(1);
    }

    if (rc1 == 0) {
        printf("(%d) rc1 - Writing to the pip\n", getpid());
        close(pip[0]);
        write(pip[1], "Hi, this is rc1\n", 17);
        exit(0);
    }

    int rc2 = fork();
    if (rc2 < 0) {
        fprintf(stderr, "fork2 failed\n");
        exit(1);
    }

    if (rc2 == 0) {
        printf("(%d) rc2 - Reading from the pip\n", getpid());
        close(pip[1]);
        read(pip[0], str, 17);
        printf("(%d) rc2 - read str = %s", getpid(), str);
    }

    close(pip[0]);
    close(pip[1]);

    waitpid(rc1, NULL, 0);
    waitpid(rc2, NULL, 0);
}