#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argv, char **argc) {
    if (argv != 2) {
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }

    char *str = argc[1];
    
    while (1) {
        sleep(1);
        printf("%s\n", str);
    }

    return 0;
}