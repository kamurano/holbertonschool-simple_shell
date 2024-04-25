#include "main.h"

#define max_len 100

void handle_command(char *command) {
	if (strncmp(command, "echo", 4) == 0)
		printf("%s\n", command + 5);
	else if (strcmp(command, "exit") == 0)
		exit(0);
	else
		printf("Unknown command: %s\n", command);
}

int main() {
	char command[max_len];

	while (1)
	{
		printf(">> ");
		fgets(command, max_len, stdin);
		command[strcspn(command, "\n")] = 0;

		handle_command(command);
	}
	return 0;
}
