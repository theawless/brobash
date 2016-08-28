#include "common.h"
#include "execute.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Spawns a child process to execute a command taking input from in fd and giving output to out fd
 * @param in input file descriptor
 * @param out output file descriptor
 * @param command structure containing information about the command
 * @return exit status of the process
 */
int process_spwanner(int in, int out, const struct simple_command command) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork error");
        return -1;
    } else if (pid == 0) {
        if (command.io[0] == 1) {
            FILE *ifp = fopen(command.str_io[0], "r");
            dup2(fileno(ifp), 0);
            close(fileno(ifp));
        } else if (in != 0) {
            dup2(in, 0);
            close(in);
        }
        if (command.io[1] == 1) {
            FILE *ofp = fopen(command.str_io[1], "w+");
            dup2(fileno(ofp), 1);
            close(fileno(ofp));

        } else if (out != 1) {
            dup2(out, 1);
            close(out);
        }
        return execute(command);
    } else {
        return pid;
    }
}

/**
 * Function to execute n pipe commands
 * @param commands contains the commands to be executed
 * @param n number of pipe separated commands
 * @return exit status
 */
int pipe_execute(const struct simple_command *commands, int n) {
    if (n == 1 && commands[0].is_builtin == 1) {
        execute(commands[0]);
        return 1;
    }
    int i, in, fd[2], status;
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork error");
        return -1;
    } else if (pid == 0) {
        in = 0;
        for (i = 0; i < n - 1; ++i) {
            pipe(fd);
            process_spwanner(in, fd[1], commands[i]);
            close(fd[1]);
            in = fd[0];
        }

        if (commands[i].io[0] == 1) {
            FILE *ifp = fopen(commands[i].str_io[0], "r");
            dup2(fileno(ifp), 0);
            close(fileno(ifp));
        } else if (in != 0) {
            dup2(in, 0);
        }

        if (commands[i].io[1] == 1) {
            FILE *ofp = fopen(commands[i].str_io[1], "w+");
            dup2(fileno(ofp), 1); //extra
            close(fileno(ofp));
        }
        execute(commands[i]);
        exit(EXIT_FAILURE);
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return 1;
    }
}