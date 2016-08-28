#include "config.h"

/**
 * @struct simple_command
 * The basic command structure.
 * @var name: command name
 * @var  args: arguments provided to the command, has command name as well
 * @var  num_args: number of args provided
 * @var  is_builtin: zero if the command is not builtin
 * @var  io: array of 3 bool elements: input, output, error. Tells whether io redirection was intended.
 * @var  str_io: File names of io redirections. input, output, error.
 */
struct simple_command {
    char *name;
    char **args;
    int num_args;
    int is_builtin;
    int io[3];
    char *str_io[3];
};

