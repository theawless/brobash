char **history;

void add_history(char *line);

char *get_recent_history();

char *get_nth_recent_history(int num);

void read_history();

void write_history();

int get_file_lines_number(FILE *fp);

char *get_nth_history(int i);
