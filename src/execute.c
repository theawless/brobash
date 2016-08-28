#include "common.h"
#include <unistd.h>
#include <stdio.h>
#include "inbuilts.h"

/**
 * Checks if the command is built in command, else make system calls.
 * @param command command to be executed.
 * @return execution status of the command.
 */
int execute(const struct simple_command command) {
    int status;
    if (command.is_builtin == 1) {
        status = builtins_execute(command);
    } else {
        status = execvp(command.name, command.args);
    }
    if (status < 0) {
        perror("brobash");
    }
    return status;
}