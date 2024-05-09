#ifndef MAINH
#define MAINH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#define MAX_LEN 100
#define PROMPT ">> "
#define PATH "PATH="
extern char **environ;
void handle_command(char *u_command);
void setup_environment(char **path_env, char **path);
void execute_command(char **args, char *path);

#endif
