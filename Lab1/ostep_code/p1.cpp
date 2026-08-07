#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char *argv[]) {
    std::cout << "hello world (pid:" << getpid() << ")" << std::endl;
    int rc = fork();
    if (rc < 0) {
        std::cerr << "fork failed" << std::endl;
        exit(1);
    } else if (rc == 0) {
        std::cout << "hello, I am child (pid:" << getpid() << ")" << std::endl;
    } else {
        std::cout << "hello, I am parent of " << rc << " (pid:" << getpid() << ")" << std::endl;
    }
    return 0;
}
