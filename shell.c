#include "main.h"
void print_env(void)
{
	char **env_ptr = environ;

	while (*env_ptr != NULL)
	{
		printf("%s\n", *env_ptr);
		env_ptr++;
	}
}
void setup_environment(char **path_env, char **path)
{
	int i;

	*path = malloc(MAX_LEN);
	if (!*path)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	memset(*path, 0, MAX_LEN);

	for (i = 0; environ[i] != NULL; i++)
		if (strncmp(environ[i], PATH, 5) == 0)
		{
			*path_env = strdup(environ[i] + 5);
			break;
		}
}
void execute_command(char **args, char *path)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
		{
			free(path);
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (wait(&status) == -1)
		{
			free(path);
			perror("wait failed");
			exit(EXIT_FAILURE);
		}
	}
}
void handle_command(char *u_command)
{
	char *args[MAX_LEN], *command = strtok(u_command, " \t");
	char *path = NULL, *path_token = NULL, *path_env = NULL;
	int i = 0, found = 0;

	setup_environment(&path_env, &path);

	args[0] = NULL;
	while (command != NULL && i < MAX_LEN - 1)
	{
		args[i] = command;
		i++;
		command = strtok(NULL, " \t");
	}
	if (args[0] == NULL)
	{
		free(path_env);
		free(path);
		return;
	}
	args[i] = NULL;

	if (strchr(args[0], '/') != NULL)
	{
		if (access(args[0], X_OK) == -1)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			free(path);
			free(path_env);
			exit(127);
		}
		free(path);
		free(path_env);
		path = strdup(args[0]);
		found = 1;
	}
	else
	{
		if (path_env == NULL)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			free(path_env);
			free(path);
			exit(127);
		}
		path_token = strtok(path_env, ":");

		while (path_token != NULL)
		{
			strcpy(path, path_token);
			strcat(path, "/");
			strcat(path, args[0]);
			if (access(path, X_OK) != -1)
			{
				found = 1;
				break;
			}
			path_token = strtok(NULL, ":");
		}
		free(path_env);
	}
	if (found == 0)
	{	
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		free(path);
		exit(127);
	}

	execute_command(args, path);
	free(path);	
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
		else if (strcmp(commands_array[a], "env") == 0)
			print_env();
		else
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
