#include "shell.h"

int main(int argc, char *argv[]) {
    char *line;
    char **args;
    int status;

    char **paths = getPaths();

    if (argc > 1) {
        // Non-interactive mode
        line = readLine();
        args = splitLine(line);
        status = executeCommand(args, paths);

        free(line);
        free(args);
    } else {
        // Interactive mode
        do {
            displayPrompt();
            line = readLine();
            args = splitLine(line);

            if (args[0] == NULL) {
                free(line);
                free(args);
                continue;
            }

            if (strcmp(args[0], "exit") == 0) {
                exitShell();
            } else if (strcmp(args[0], "env") == 0) {
                printEnvironment();
            } else {
                status = executeCommand(args, paths);
            }

            free(line);
            free(args);
        } while (status);
    }

    free(paths);
    return 0;
}

