#include "main.h"
#include <dirent.h>

#define MAX_LEN 100
#define PROMPT ">> "

void handle_command(char *command)
{
	if (strncmp(command, "echo", 4) == 0)
		printf("%s\n", command + 5);
	else if (strcmp(command, "exit") == 0)
		exit(0);
	else if (strncmp(command, "ls", 2) == 0)
	{
		DIR *dir;
		struct dirent *ent;

		dir = opendir(".");
		if (dir != NULL)
		{
			while ((ent = readdir(dir)) != NULL)
				printf("%s\n", ent->d_name);
			closedir(dir);
		}
		else
		{
			perror("");
		}
	}
	else
		printf("Unknown command: %s\n", command);
}

int main(void)
{
	char command[MAX_LEN];

	while (1)
	{
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		if (fgets(command, MAX_LEN, stdin) == NULL)
		{
			if (feof(stdin))
				break;
			perror("Error reading command");
			continue;
		}

		command[strcspn(command, "\n")] = '\0';
		handle_command(command);
	}
	return (0);
}
