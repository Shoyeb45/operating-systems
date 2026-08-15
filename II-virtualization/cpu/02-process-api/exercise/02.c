#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int file_desc = open("file.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    int rc = fork();

    if (rc < 0 || file_desc < 0) {
        fprintf(stderr, "fork failed pr opening a file failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("[%d] File is being written by child process\n", getpid());
        write(file_desc, "File written by child\n", 23);
    } else {
        printf("[%d] File is being written by parent process\n", getpid());
        write(file_desc, "File written by parent\n", 23);
    }
    
    return 0;
}