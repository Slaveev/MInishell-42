/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:32:56 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/14 13:10:52 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	handle_pwd(void)
{
	char	pwd[1024];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		printf("%s\n", pwd);
	else
		perror("getcwd() error");
}

void	handle_cd(char **input)
{
	char	*dir;
	char	new_path[1024];

	dir = input[1];
	while (dir != NULL && (!strcmp(dir, " ") || !strcmp(dir, "\t")))
		dir = *(++input);
	if (dir == NULL || strlen(dir) == 0)
		dir = getenv("HOME");
	if (chdir(dir) != 0)
		perror("chdir() error");
	else
		if (getcwd(new_path, sizeof(new_path)) == NULL)
			perror("getcwd() error");
}

void	handle_echo(char **input)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;
	while (input[i] && !strcmp(input[i], "-n"))
	{
		new_line = 0;
		i++;
	}
	while (input[i])
	{
		printf("%s", input[i]);
		if (input[i + 1] && input[i][0] != '\0')
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
}

void	handle_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	builtin_exec(char **input, char **env)
{
	if (!strncmp(input[0], "env", 4))
		handle_env(env);
	else if (!strncmp(input[0], "exit", 4) && input[1] == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	else if (!strncmp(input[0], "echo", 4))
		handle_echo(input);
	else if (!strncmp(input[0], "pwd", 3) && input[1] == NULL)
		handle_pwd();
	else if (!strncmp(input[0], "cd", 2))
		handle_cd(input);
	else
		printf("Command not found: %s\n", input[0]);
}
