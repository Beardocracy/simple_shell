#include "holberton.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

/**
 * main - a simple shell
 * @ac: the number of arguments
 * @av: an array of strings taken in as arguments from the command line.
 * @env: an array of strings, the environmental variables.
 * Return: need to ammend.
 */
int main(int ac, char *av[], char *env[])
{
	char *buffer, *com[32], *com_path;
	char delim = ' ';
	ssize_t num_read;
	size_t n = 0;
	int i, flag, path_value;

	while (num_read != -1)
	{
		print_prompt();
		num_read = getline(&buffer, &n, stdin);
		if (num_read == -1)
			exit_status(com_path, &buffer);
		del_newline(buffer);
		com[0] = strtok(buffer, &delim);
		for (i = 1, flag = 1; flag; i++)
		{
			com[i] = strtok(NULL, &delim);
			if (com[i] == NULL)
				flag = 0;
		}
		com_path = get_path(com[0], env, &path_value);
		switcher(path_value, com_path, com, env, av);
		free(com_path);
	}
	if (buffer)
		free(buffer);
	return (0);
}
