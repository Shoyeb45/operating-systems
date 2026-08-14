#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd = open("/tmp/file", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    assert(fd > -1);

    int rc = write(fd, "hello world\n", 13);
    assert(rc == 13);

    close(fd);
    return 0;
}