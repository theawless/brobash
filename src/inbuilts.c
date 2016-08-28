#include "common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"
#include "brobash.h"

/**
 * Commands list.
 * Contains all built in commands.
 * Contains some non-built in commands.
 */
struct simple_command command_list[] = {
        {"type",    NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"cd",      NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"help",    NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"exit",    NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"recent",  NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"history", NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"!!",      NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"!",       NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"@@",      NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"@",       NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"??",      NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
        {"?",       NULL, 0, 1, {0, 0, 0}, {NULL, NULL, NULL}},
};

/**
 * @return The size of command_list[].
 */
int command_list_length() {
    return sizeof(command_list) / sizeof(struct simple_command);
}

/**
 * Executes the built in command.
 * @param command
 * @return Execution status of built in command.
 */
int builtins_execute(const struct simple_command command) {
    if (strcmp(command.name, "help") == 0) {
        int i;
        printf("Team BrOS's shell implementation\n");
        printf("Type program names and arguments, and hit enter.\n");
        printf("The following commands are built in:\n");
        for (i = 0; i < command_list_length(); i++) {
            if (command_list[i].is_builtin == 0) {
                break;
            }
            printf("%s\n", command_list[i].name);
        }
        printf("Type man <command> to get information about that command.\n");
        return 1;
    } else if (strcmp(command.name, "exit") == 0) {
        write_history();
        exit(EXIT_SUCCESS);
    } else if (strcmp(command.name, "cd") == 0) {
        if (command.args[1] == NULL) {
            fprintf(stderr, "lsh: expected argument to \"cd\"\n");
        } else {
            if (chdir(command.args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    } else if (strcmp(command.name, "type") == 0) {
        int i, j, flag;
        for (i = 1; i < command.num_args; i++) {
            flag = 0;
            for (j = 0; j < command_list_length(); j++) {
                if (strcmp(command.args[i], command_list[i].name) == 0 && command_list[i].is_builtin == 1) {
                    printf("%s is a built in command.\n", command.args[i]);
                    flag = 1;
                    break;
                }
            }
            if (!flag) {
                printf("%s is a system command.\n", command.args[i]);
            }
        }
    } else if (strcmp(command.name, "recent") == 0) {
        int i;
        for (i = HIST_SIZE; i > 0; i--) {
            char *line = get_nth_recent_history(i);
            if (line && strcmp(line, "\n") != 0) {
                printf("%d: %s", i, line);
            }
        }
    } else if (strcmp(command.name, "@@") == 0) {
        char *str_command = get_recent_history();
        process_and_execute_line(str_command);
    } else if (strcmp(command.name, "@") == 0) {
        if (command.args[1] != NULL) {
            int i = atoi(command.args[1]);
            if (i > 0) {
                char *str_command = get_nth_recent_history(i);
                printf("%s", str_command);
                process_and_execute_line(str_command);
            } else {
                printf("Integer should be greater than 0");
            }
        } else {
            printf("Require second arguement.");
        }
    } else if (strcmp(command.name, "??") == 0) {

        printf("%s", get_recent_history());
    } else if (strcmp(command.name, "?") == 0) {
        if (command.args[1] != NULL) {
            int i = atoi(command.args[1]);
            if (i > 0) {
                printf("%s", get_nth_recent_history(i));
            } else {
                printf("Integer should be greater than 0");
            }
        } else {
            printf("Require second arguement.");
        }
    } else if (strcmp(command.name, "!!") == 0) {
        char *str_command = get_recent_history();
        process_and_execute_line(str_command);
    } else if (strcmp(command.name, "!") == 0) {
        if (command.args[1] != NULL) {
            int i = atoi(command.args[1]);
            if (i > 0) {
                char *str_command = get_nth_history(i);
                printf("%s", str_command);
                process_and_execute_line(str_command);
            } else {
                printf("Integer should be greater than 0");
            }
        } else {
            printf("Require second arguement.");
        }
    } else if (strcmp(command.name, "history") == 0) {
        int i = 0, lines;
        FILE *hist = fopen("hist", "a+");
        if (hist) {
            lines = get_file_lines_number(hist);
            for (i = lines; i > 0; i--) {
                printf("%d: %s", i, get_nth_history(i));
            }
        }
    }
    return 0;
}