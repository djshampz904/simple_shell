#include "shell.h"

char **splitLine(char *line) {
    const char *delimiters = " \t\r\n";
    const int initialSize = 64;
    int size = initialSize;
    int position = 0;
    char **tokens = malloc(size * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delimiters);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= size) {
            size += initialSize;
            tokens = realloc(tokens, size * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "Reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delimiters);
    }
    tokens[position] = NULL;
    return tokens;
}