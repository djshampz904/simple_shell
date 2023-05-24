#include "shell.h"

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