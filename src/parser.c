#include "common.h"
#include "inbuilts.h"
#include <stdlib.h>
#include <string.h>

/**
 * Finds the type of the given command from the commands list.
 * @param command to find the type of this command.
 */
void find_type(struct simple_command *command) {
    int i;
    for (i = 0; i < command_list_length(); i++) {
        if (strcmp(command->name, command_list[i].name) == 0 && command_list[i].is_builtin == 1) {
            command->is_builtin = 1;
            return;
        }
    }
    command->is_builtin = 0;
}

/**
 * Parses the line to find all commands.
 * Tokenize by pipes to find commands.
 * Tokenize by whitespace to find arguements.
 * If IO redirection is found, edit the command struct.
 * @param line parses this line
 * @param num_commands out param, number of commands.
 * @return array of commands.
 */
struct simple_command *parse_line(char *line, int *num_commands) {
    int position = 0, position2 = 0;
    struct simple_command *commands = (struct simple_command *) malloc(COMMAND_SIZE * sizeof(struct simple_command));
    char *token, *token2, **args;
    token = strtok_r(line, PIPE_DELIM, &line);
    while (token) {
        args = (char **) malloc(ARGS_SIZE * sizeof(char *));
        char *temp_token = strdup(token);
        token2 = strtok_r(temp_token, ARGS_DELIMS, &temp_token);
        commands[position].name = token2;
        commands[position].io[0] = 0;
        commands[position].io[1] = 0;
        commands[position].io[2] = 0;
        position2 = 0;
        int take_in = -1;
        while (token2) {
            if (take_in == 0) {
                commands[position].str_io[0] = token2;
                take_in = -1;
            } else if (take_in == 1) {
                commands[position].str_io[1] = token2;
                take_in = -1;
            } else if (take_in == 2) {
                commands[position].str_io[2] = token2;
                take_in = -1;
            } else if (strcmp(token2, "<") == 0) {
                commands[position].io[0] = 1;
                take_in = 0;
            } else if (strcmp(token2, ">") == 0) {
                commands[position].io[1] = 1;
                take_in = 1;
            } else if (strcmp(token2, "&>") == 0) {
                commands[position].io[2] = 1;
                take_in = 2;
            } else {
                args[position2] = token2;
                position2++;
            }
            token2 = strtok_r(temp_token, ARGS_DELIMS, &temp_token);
        }
        commands[position].args = args;
        commands[position].num_args = position2;
        find_type(&commands[position]);
        token = strtok_r(line, PIPE_DELIM, &line);
        position++;
    }
    *num_commands = position;
    return commands;
}
