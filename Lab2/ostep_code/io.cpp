// OSTEP persistence demo - write data to a file
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

int main() {
    int fd = open("/tmp/file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        cout << "Failed to open file!" << endl;
        return 1;
    }

    char buf[100];
    sprintf(buf, "Hello from OSTEP IO demo (PID: %d)\n", getpid());
    write(fd, buf, strlen(buf));
    fsync(fd);
    close(fd);

    cout << "Wrote data to /tmp/file.txt successfully." << endl;
    return 0;
}
