#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    ///check that we have an input at all
    if (argc < 2) {
        fprintf(stderr, "Usage: mypipe <message>\n");
        exit(EXIT_FAILURE);
    }
    
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    
    //Child process
    if (pid == 0) {
        char buffer[1024];
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("%s\n", buffer);
        close(fd[0]);
    //Parent process    
    } else {
        int len = strlen(argv[1]) + 1;
        close(fd[0]);
        write(fd[1], argv[1], len);
        close(fd[1]);
    }
    return 0;
}