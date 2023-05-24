#include "shell.h"

int main(int argc, char *argv[]){
	char *line, **args;
	char **paths;
	int status;
	UNUSED(argv);
	
	paths = getPaths();

    if (argc > 1) {
        line = readLine();
        args = splitLine(line);
        status = executeCommand(args, paths);

        free(line);
        free(args);
    } else {
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

