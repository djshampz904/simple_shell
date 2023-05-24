#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void displayPrompt() {
    printf("($) ");
    fflush(stdout);
}

char *readLine() {
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

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

char **getPaths(void)
{
    char *envPath = getenv("PATH");
    int numPaths = 0;
    char **paths = malloc(BUFFER_SIZE * sizeof(char *));
    char *path;

    if (!paths)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    path = strtok(envPath, ":");
    while (path != NULL)
    {
        paths[numPaths] = path;
        numPaths++;

        if (numPaths >= BUFFER_SIZE)
        {
            perror("Too many paths in PATH environment variable");
            exit(EXIT_FAILURE);
        }

        path = strtok(NULL, ":");
    }
    paths[numPaths] = NULL;

    return paths;
}

char *getCommandPath(char *command, char **paths)
{
    char *commandPath = malloc(BUFFER_SIZE * sizeof(char));
    if (!commandPath)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (access(command, F_OK) == 0)
    {
        strncpy(commandPath, command, BUFFER_SIZE);
        return commandPath;
    }

    for (int i = 0; paths[i] != NULL; i++)
    {
        snprintf(commandPath, BUFFER_SIZE, "%s/%s", paths[i], command);
        if (access(commandPath, F_OK) == 0)
            return commandPath;
    }

    free(commandPath);
    return NULL;
}

int executeCommand(char **args, char **paths)
{
    char *commandPath;
    pid_t childPid, wpid;
    int status;

    commandPath = getCommandPath(args[0], paths);
    if (commandPath == NULL)
    {
        fprintf(stderr, "%s: command not found\n", args[0]);
        return 1;
    }

    childPid = fork();
    if (childPid == -1)
    {
        perror("fork");
        return 0;
    }
    else if (childPid == 0)
    {
        if (execv(commandPath, args) == -1)
        {
            perror("execv");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        do {
            wpid = waitpid(childPid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    free(commandPath);
    return 1;
}

void exitShell() {
    exit(EXIT_SUCCESS);
}

void printEnvironment() {
    extern char **environ;
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
}

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

