#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void displayPrompt(void);

char *readLine(void);

char **splitLine(char *line);

char **getPaths(void);

char *getCommandPath(char *command, char **paths);

int executeCommand(char **args, char **paths);

void exitShell(void);

void printEnvironment(void);
#endif
