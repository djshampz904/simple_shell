#include "shell.h"

int executeCommand(char **args, char **paths)
{
    char *commandPath;
    pid_t childPid;
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
	    pid_t wpid;
	    UNUSED(wpid);
        do {
            wpid = waitpid(childPid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    free(commandPath);
    return 1;
}
