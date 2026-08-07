#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    std::cout << "hello world (pid:" << getpid() << ")" << std::endl;
    int rc = fork();
    if (rc < 0) {
        std::cerr << "fork failed" << std::endl;
        exit(1);
    } else if (rc == 0) {
        std::cout << "hello, I am child (pid:" << getpid() << ")" << std::endl;
        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("p3.cpp");
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
        std::cout << "this shouldn't print out" << std::endl;
    } else {
        int wc = wait(NULL);
        std::cout << "hello, I am parent of " << rc << " (wc:" << wc << ") (pid:" << getpid() << ")" << std::endl;
    }
    return 0;
}
