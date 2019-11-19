#include "holberton.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * _strlen - returns the length of a string
 * @s: the string
 * Return: the length of the string
 */
int _strlen(char *s)
{
	int i;

	for (i = 0; s[i]; i++)
		;
	return (i);
}

/**
 * path_combo - combines 2 strings, with a / in between them.
 * @dir: first part.
 * @comm: part 2.
 * Return: malloc'd string.
 */
char *path_combo(char *dir, char *comm)
{
	char *cat;
	int i, j;

	cat = malloc(sizeof(char) * (2 + _strlen(dir) + _strlen(comm)));
	if (cat == NULL)
		exit(33);
	for (i = 0; dir[i]; i++)
		cat[i] = dir[i];
	cat[i] = '/';
	for (i++, j = 0; comm[j]; i++, j++)
		cat[i] = comm[j];
	cat[i] = '\0';
	return (cat);
}

/**
 * env_path_parse - parses the path line from the environment variables.
 * @env: the array of strings with all environment variables.
 * Return: a mallocd string containing the environment variable line.
 */
char *env_path_parse(char **env)
{
	int i, j, flag;
	char path[6] = "PATH=";
	char *path_line;

	for (i = 0; env[i]; i++)
	{
		for (j = 0, flag = 1; flag && j < 5; j++)
		{
			if (env[i][j] != path[j])
				flag = 0;
		}
		if (flag)
		{
			for (j = 0; env[i][j] != '\0'; j++)
				;
			path_line = malloc(sizeof(char) * (j + 1));
			if (path_line == NULL)
			{
				return (NULL);
			}
			for (j = 0; env[i][j]; j++)
				path_line[j] = env[i][j + 5];
			path_line[j] = '\0';
		}
	}
	return (path_line);
}
/**
 * get_path - returns absolute path to a command.
 * @comm: the command string
 * @env: the environment variables, in an array of strings.
 * @ret_value: int pointer to be changed depending on conditions.
 * Return: the absolute path to the command, if it exists. 
 * 	If no file exists, return bad command.
 * Description:	ret_value will return...
 * 	1 if path exists and permission is ok
 * 	0 if path exists and no permission allowed
 * 	-1 if no path exists (command not found)
 */
char *get_path(char *comm, char **env, int *ret_value)
{
	int i, flag, acc_ret;
	char *pathlist[16];
	char *paths_in;
	char *cat_temp;
	char delim = ':';

	*ret_value = -1;
	if (comm[0] == '/' || comm[0] == '.')
	{
		if (access(comm, F_OK) == 0)
		{	
			*ret_value = 0;
			if (access(comm, X_OK) == 0)
				*ret_value = 1;
		}
		return (comm);
	}
	paths_in = env_path_parse(env);
	pathlist[0] = strtok(paths_in, &delim);
	for (i = 1, flag = 1; flag; i++)
	{
		pathlist[i] = strtok(NULL, &delim);
		if (pathlist[i] == NULL)
			flag = 0;
	}
	for (i = 0; pathlist[i]; i++)
	{
		cat_temp = path_combo(pathlist[i], comm);
		acc_ret = access(cat_temp, F_OK);
		if (acc_ret == 0)
		{
			*ret_value = 0;
			acc_ret = access(cat_temp, F_OK | X_OK);
			if (acc_ret == 0)
				*ret_value = 1;
			return (cat_temp);
		}
	}
	free(cat_temp);
	return (comm);
}
