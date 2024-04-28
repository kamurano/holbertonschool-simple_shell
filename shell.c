#include "main.h"
#include <dirent.h>

#define MAX_LEN 100
#define PROMPT ">> "

void handle_command(char *u_command)
{
	char *args[MAX_LEN], *command = strtok(u_command, " ");
	pid_t pid;
	int status, i = 0;

	while (command != NULL && i < MAX_LEN - 1)
	{
		args[i] = command;
		i++;
		command = strtok(NULL, " ");
	}
	args[i] = NULL;
	
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return;
	}
	else if (!pid)
	{
		if (execve(args[0], args, NULL) == -1)
		{
			perror("Execve failed");
			exit(0);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("Waitpid failed");
			exit(0);
		}
	}
}

int main(void)
{
	char command[MAX_LEN];

	while (1)
	{
		ssize_t read_size;
		
		printf(PROMPT);

		read_size = read(STDIN_FILENO, command, MAX_LEN);
		if (read_size == -1)
		{
			perror("Error reading command");
			exit(0);
		}
		else if (read_size == 0)
			break;

		command[read_size - 1] = '\0';
		handle_command(command);
	}
	return (0);
}
