#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd = open("/tmp/file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    assert(fd >= 0);
    
    char buffer[100];
    sprintf(buffer, "Hello World from OSTEP Persistence Demo (PID: %d)\n", getpid());
    
    int rc = write(fd, buffer, strlen(buffer));
    assert(rc == (int) strlen(buffer));
    
    fsync(fd);
    close(fd);
    
    printf("Successfully wrote persistent data to /tmp/file.txt\n");
    return 0;
}
