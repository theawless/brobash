#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int end = 0;
/**
 * @var
 * Array of strings, used as a queue.
 */
char **history = NULL;

/**
 * Adds a line to the history. Removes the old history.
 * @param line to be added in history.
 */
void add_history(char *line) {
    if (!history) {
        history = (char **) malloc(HIST_SIZE * sizeof(char *));
        int i = 0;
        for (i = 0; i < HIST_SIZE; i++) {
            history[i] = NULL;
        }
    }
    if ((strcmp(line, "history\n") == 0) || (strcmp(line, "recent\n") == 0) || line == strchr(line, '@') ||
        line == strchr(line, '!') ||
        line == strchr(line, '?')) {
        return;
    }
    if (history[end]) {
        free(history[end]);
    }
    history[end] = line;
    end = (end + 1) % HIST_SIZE;
    //save to history
    FILE *log = fopen("hist", "a+");
    if (log) {
        fprintf(log, "%s", line);
    }
    fclose(log);
}

/**
 * Writes the recents history from history array to a file.
 */
void write_history() {
    int i;
    FILE *hist = fopen("recent", "a+");
    if (hist) {
        for (i = 0; i < HIST_SIZE; i++) {
            if (history[i] && (strcmp(history[i], "") != 0)) {
                fprintf(hist, "%s", history[i]);
            }
        }
    }
    fclose(hist);
}

/**
 * Reads the recents history from hist file and fills up the history array.
 */
void read_history() {
    ssize_t read_status;
    FILE *hist = fopen("recent", "r");
    if (hist) {
        do {
            char *line;
            size_t size;
            read_status = getline(&line, &size, hist);
            if (read_status != -1) {
                add_history(strdup(line));
            }
        } while (read_status != -1);
    }
    fclose(hist);
}

/**
 * Return the most recent command.
 * @return string format of recent command.
 */
char *get_recent_history() {
    if (history) {
        int i = end - 1;
        if (i < 0) {
            i += HIST_SIZE;
        }
        return history[i];
    } else return "";
}

/**
 * Gives the number of lines in the specified file.
 * @param fp the file whose number of lines is to be found out.
 * @return the number of lines.
 */
int get_file_lines_number(FILE *fp) {
    int lines = 0, ch;
    while (!feof(fp)) {
        ch = fgetc(fp);
        if (ch == '\n') {
            lines++;
        }
    }
    rewind(fp);
    return lines;
};

/**
 * get the nth command from history
 * @param num
 * @return string of command
 */
char *get_nth_history(int num) {
    int ch, nlines = 0, i = 0;
    FILE *hist = fopen("hist", "r");
    if (!hist) {
        return NULL;
    }
    while (!feof(hist) && nlines != num - 1) {
        ch = fgetc(hist);
        if (ch == '\n') {
            nlines++;
        }
    }
    if (feof(hist)) {
        return NULL;
    }
    ssize_t status;
    size_t size;
    char *line = NULL;
    status = getline(&line, &size, hist);
    if (status == -1) {
        return NULL;
    }
    //fclose(hist);
    return strdup(line);
}

/**
 * get the nth command from recents history.
 *@param num
 * @return string of command
 */
char *get_nth_recent_history(int num) {
    if (num <= HIST_SIZE && history) {
        int i = end - num;
        if (i < 0) {
            i += HIST_SIZE;
        }
        return history[i];
    } else {
        return "";
    }
}

