#include "common.h"
#include "piping.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/**
 * Reads the line from whichever stream that was provided.
 * @param stream
 * @param line_size Out param.
 * @param read_status Out param.
 * @return line
 */
char *read_line(FILE *stream, size_t *line_size, ssize_t *read_status) {
    char *line = NULL;
    size_t size = 0;
    *read_status = getline(&line, &size, stream);
    while (line[0] == ' ' || line[0] == '\t') {
        line++;
        size--;
    }
    *line_size = size;
    return line;
}

/**
 * Process and execute the line returned by read_line.
 * @param line line to be processed and execute.
 * @return returns the status of executed command.
 */
int process_and_execute_line(char *line) {
    int num_commands;
    struct simple_command *commands = parse_line(line, &num_commands);
    int status = pipe_execute(commands, num_commands);
    //free(line);
    //free(commands);
    return status;
}

/**
 * Main loop of the shell, displays a prompt, accepts and runs commands.
 * @param stream Stream can be stdin or a file pointer.
 */
void run_loop(FILE *stream) {
    int status;
    ssize_t read_status;
    size_t line_size;
    char *line;
    do {
        if (stream == stdin) {
            printf(COMMAND_STYLE);
        }
        line = read_line(stream, &line_size, &read_status);
        if (read_status == -1) {
            return;
        }
        if (line[0] == '\n') {
            continue;
        }
        char *temp = strdup(line);
        char *temp2 = strdup(line);
        add_history(temp);
        status = process_and_execute_line(temp2);
        //fprintf(stderr, "%d", getpid());
        //free(line);
    } while (status);
}

/**
 * Enters the program from here.
 * @param argc Number of args.
 * @param argv File to take as input, if provided.
 * @return exit status
 */
int main(int argc, char **argv) {
    //read_history();
    FILE *stream = fopen("init", "r");
    if (stream) {
        run_loop(stream);
        //fprintf(stderr, "%d", getpid());
        //fclose(stream);
    }
    if (argc == 1) {
        run_loop(stdin);
    } else if (argc == 2) {
        fprintf(stderr, "%s", argv[1]);
        stream = fopen(argv[1], "r");
        if (stream) {
            run_loop(stream);
            //fprintf(stderr, "%d", getpid());
            //fclose(stream);
        }
    } else if (argc > 2) {
        fprintf(stderr, "Too many arguments supplied!\n");
    }
    write_history();
    return EXIT_SUCCESS;
}