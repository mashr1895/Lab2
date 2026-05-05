#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <linux/limits.h>  // for PATH_MAX
#include "LineParser.h"
#include <sys/wait.h>
#include <fcntl.h>

void execute(cmdLine *pCmdLine, int debug) {
    pid_t pid = fork();
    if (pid == 0) {
        ///checking for input redirect
        if (pCmdLine->inputRedirect != NULL) {
            int fd = open(pCmdLine->inputRedirect, O_RDONLY);
            close(0);
            dup(fd);
            close(fd);
        }
        ///checking for output redirect
        if (pCmdLine->outputRedirect != NULL) {
            int fd = open(pCmdLine->outputRedirect, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            close(1);
            dup(fd);
            close(fd);
        }
        execvp(pCmdLine->arguments[0], (char *const *)pCmdLine->arguments);
        perror("execvp failed");
        _exit(EXIT_FAILURE);
    } else {
        if (debug)
            fprintf(stderr, "PID: %d, Program: %s, %s\n",
                pid,
                pCmdLine->arguments[0],
                pCmdLine->blocking ? "Foreground" : "Background");
        if (pCmdLine->blocking)
            waitpid(pid, NULL, 0);
    } 
}

int main(int argc, char **argv) {
    char cwd[PATH_MAX];   //hold the current directory path
    char input[2048];     //hold what the user types
    cmdLine *parsedLine;  //hold the parsed result
    int debugMode = (argc > 1 && strcmp(argv[1], "-d") == 0);

    //Infinite loop like in a real shell, until it breaks
     while (1) {
        ///saving the directory and print it with > in the end
        getcwd(cwd, sizeof(cwd));
        printf("%s> ", cwd);
        fflush(stdout);

        ///in the case we get a NULL input
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        ///we know the input is not null, so we can parse it to cmdLine struct
        parsedLine = parseCmdLines(input);

        ///if the struct is NULL, we just do nothing - restarts the loop
        if (parsedLine == NULL)
            continue;

        ///if the input was "quit" we release the memory parsedLine uses, and exit.
        if (strcmp(parsedLine->arguments[0], "quit") == 0) {
            freeCmdLines(parsedLine);
            exit(EXIT_SUCCESS);
        }

        ///if there is an actual command - we execute it with execute()

        ///check if the command is 'cd'
        if (strcmp(parsedLine->arguments[0], "cd") == 0) {
            if (chdir(parsedLine->arguments[1]) != 0)
                perror("cd failed");
            freeCmdLines(parsedLine);
            continue;
        }
        ///check if command is 'stop'
        if (strcmp(parsedLine->arguments[0], "stop") == 0) {
            kill(atoi(parsedLine->arguments[1]), SIGSTOP);
            freeCmdLines(parsedLine);
            continue;
        }
        ///check if command is 'wakeup'
        if (strcmp(parsedLine->arguments[0], "wakeup") == 0) {
            kill(atoi(parsedLine->arguments[1]), SIGCONT);
            freeCmdLines(parsedLine);
            continue;
        }
        ///check if command is 'ice'
        if (strcmp(parsedLine->arguments[0], "ice") == 0) {
            kill(atoi(parsedLine->arguments[1]), SIGINT);
            freeCmdLines(parsedLine);
            continue;
        }
        ///check if command is 'nuke'
        if (strcmp(parsedLine->arguments[0], "nuke") == 0) {
            kill(-atoi(parsedLine->arguments[1]), SIGKILL);
            freeCmdLines(parsedLine);
            continue;
        }

        ///executing the command
        execute(parsedLine, debugMode);
        freeCmdLines(parsedLine);
    }

    return 0;
}