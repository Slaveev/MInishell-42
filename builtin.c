/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:32:56 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/26 14:14:41 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

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

void print_env_vars(t_env *env)
{
    t_env_var *current = env->vars;

    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}

void	handle_export(char **input, t_env *env)
{
	char **key_value;

	if (input[1] == NULL)
		print_env_vars(env);
	else
	{
		key_value = ft_split(input[1], '=');
		if (key_value[0] && key_value[1])
			set_env_var(env, key_value[0], key_value[1]);
	}
}

void	handle_unset(char **input, t_env *env)
{
	if (input[1] != NULL)
		unset_env_var(env, input[1]);
}

void	builtin_exec(char **input, char *env)
{
	if (!strncmp(input[0], "env", 4))
		handle_env((char **)env);
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
	else if (!strncmp(input[0], "export", 6))
		handle_export(input, env);
	else if (!strncmp(input[0], "unset", 5))
		handle_unset(input, env);
	else
		printf("Command not found: %s\n", input[0]);
}
