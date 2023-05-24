#include "shell.h"

char *getCommandPath(char *command, char **paths)
{
	int i;

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

    for (i = 0; paths[i] != NULL; i++)
    {
        snprintf(commandPath, BUFFER_SIZE, "%s/%s", paths[i], command);
        if (access(commandPath, F_OK) == 0)
            return commandPath;
    }

    free(commandPath);
    return NULL;
}
