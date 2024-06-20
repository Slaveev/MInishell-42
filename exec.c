/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:41:17 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/20 12:50:52 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_execute(t_cmd *cmd, char **env)
{
	char *cmd_path;
	int fd_input, fd_output;

	if (cmd->fd_in)
	{
		fd_input = open(cmd->fd_in, O_RDONLY);
		if (fd_input == -1)
			ft_error("Failed to open input file", 1);
		if (dup2(fd_input, STDIN_FILENO) == -1)
			ft_error("Failed to redirect stdin", 1);
		close(fd_input);
	}
	if (cmd->fd_out)
	{
		printf("cmd->fd_out: %s\n", cmd->fd_out);
		fd_output = open(cmd->fd_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_output == -1)
			ft_error("Failed to open output file", 1);
		if (dup2(fd_output, STDOUT_FILENO) == -1)
			ft_error("Failed to redirect stdout", 1);
		close(fd_output);
	}
	if (is_builtin(cmd->command))
	{
		builtin_exec(cmd->args, env);
		return;
	}
	cmd_path = get_cmd_path(cmd->command, env);
	printf("cmd_path: %s\n", cmd_path);
	if (!cmd_path)
		ft_error("Command not found", 127);
	printf("ok\n");
	cmd->pid = fork();
	if (cmd->pid == -1)
		ft_error("Failed to fork", 1);
	else if (cmd->pid == 0)
	{
		if (execve(cmd_path, cmd->args, env) == -1)
			ft_error("Command not executable", 126);
	}
	else
		waitpid(cmd->pid, &cmd->status, 0);
}
