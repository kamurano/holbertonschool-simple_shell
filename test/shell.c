#include "main.h"

#define MAX_LEN 100
#define PROMPT ">> "

void handle_command(char *u_command)
{
	char *args[MAX_LEN], *command = strtok(u_command, " \t");
	pid_t pid;
	int status, i = 0;

	args[0] = NULL;
	while (command != NULL && i < MAX_LEN - 1)
	{
		args[i] = command;
		i++;
		command = strtok(NULL, " \t");
	}

	if (args[0] == NULL)
		return;
	
	args[i] = NULL;
	
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return;
	}
	else if (!pid)
	{
		if (strcmp(args[0], "env") == 0)
                {
                        char **env = environ;

                        while (*env != NULL)
                        {
                                printf("%s\n", *env);
                                env++;
                        }
                        exit(EXIT_SUCCESS);
                }
                if (strchr(args[0], '/') != NULL)
                {
                        if (access(args[0], X_OK) == 0)
                        {
                                if (execve(args[0], args, environ) == -1)
                                {
                                        perror("Execve failed");
                                        exit(EXIT_FAILURE);
                                }
                        }
                }
		printf("Command not found: %s\n", args[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("Waitpid failed");
			exit(EXIT_FAILURE);
		}
	}
}

int main(void)
{
	char commands[MAX_LEN];
	char *command;
	char *commands_array[MAX_LEN];
	int a = 0, status = 0;

	while (1)
	{
		ssize_t read_size;

		read_size = read(STDIN_FILENO, commands, MAX_LEN);
		if (read_size == -1)
		{
			perror("Error reading command");
			exit(EXIT_FAILURE);
		}
		else if (read_size == 0)
			break;
		commands[read_size] = '\0';
		a = 0;
		command = strtok(commands, "\n");
		while (command != NULL)
		{
			commands_array[a] = command;
			command = strtok(NULL, "\n");
			a++;
		}
		commands_array[a] = NULL;
		
		a = 0;
		if (strcmp(commands_array[a], "exit") == 0)
			exit(status);
		while (commands_array[a] != NULL)
		{
			command = commands_array[a];
			if (strcmp(command, "exit") == 0 && a > 0)
				exit(2);
			handle_command(command);
			a++;
		}
	}
	return (0);
}
