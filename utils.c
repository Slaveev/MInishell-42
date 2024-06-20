/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:42:14 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/20 12:45:25 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_fd(int *pfd)
{
	close(pfd[0]);
	close(pfd[1]);
}

void	ft_error(const char *msg, int status)
{
	perror(msg);
	exit(status);
}

void	free_str_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*get_cmd_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*path;
	char	*tmp;

	i = 0;
	if (env == NULL)
		return (NULL);
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 5))
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK | F_OK) == 0)
			return (free_str_array(paths), path);
		free(path);
		i++;
	}
	return (free_str_array(paths), NULL);
}

int	is_builtin(char *command)
{
	char	*builtins[] = {"cd", "pwd", "echo", "exit", "env", "export", "unset"};
	int		i;
	int		num_builtins;

	if (command == NULL)
		return (0);
	i = 0;
	num_builtins = sizeof(builtins) / sizeof(char *);
	while (i < num_builtins)
	{
		if (strcmp(command, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}
