#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("hello\n");
    } else {
        // without using wait, I used sleep and then hello is printed first and then Good bye
        // hahahahahaha....
        sleep(1);
        printf("Good bye\n");
    }
    return 0;
}