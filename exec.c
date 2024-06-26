/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:41:17 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/25 12:42:25 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void ft_execute(t_cmd *cmd, char **env)
// {
// 	char *cmd_path;
// 	int fd_input, fd_output;
// 	int	pipe_fd[2];

// 	printf("cmd->command: %s\n", cmd->command);
// 	if (cmd->fd_in)
// 	{
// 		fd_input = open(cmd->fd_in, O_RDONLY);
// 		if (fd_input == -1)
// 			ft_error("Failed to open input file", 1);
// 		if (dup2(fd_input, STDIN_FILENO) == -1)
// 			ft_error("Failed to redirect stdin", 1);
// 		close(fd_input);
// 	}
// 	if (cmd->fd_out)
// 	{
// 		printf("cmd->fd_out: %s\n", cmd->fd_out);
// 		fd_output = open(cmd->fd_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (fd_output == -1)
// 			ft_error("Failed to open output file", 1);
// 		if (dup2(fd_output, STDOUT_FILENO) == -1)
// 			ft_error("Failed to redirect stdout", 1);
// 		close(fd_output);
// 	}
// 	if (is_builtin(cmd->command))
// 	{
// 		builtin_exec(cmd->args, env);
// 		return;
// 	}
// 	if (cmd->pipe)
// 	{
// 		if (pipe(pipe_fd) == -1)
// 			ft_error("Failed to create pipe", 1);
// 	}
// 	cmd->pid = fork();
// 	if (cmd->pid == 0)
// 	{
// 		if (cmd->pipe)
// 		{
// 			close(pipe_fd[0]);
// 			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
// 				ft_error("Failed to redirect stdout", 1);
// 			close(pipe_fd[1]);
// 		}
// 		printf("cmd->pid: %d\n", cmd->pid);
// 		if (cmd->fd_in)
// 		{
// 			if (dup2(fd_input, STDIN_FILENO) == -1)
// 				ft_error("Failed to redirect stdin", 1);
// 			close(fd_input);
// 		}
// 		cmd_path = get_cmd_path(cmd->command, env);
// 		printf("cmd_path: %s\n", cmd_path);
// 		if (!cmd_path)
// 			ft_error("Command not found", 127);
// 		if (execve(cmd_path, cmd->args, env) == -1)
// 			ft_error("Command not executable", 126);
// 	}
// 	waitpid(cmd->pid, &cmd->status, 0);
// 	printf("finished fml\n");
// }

char    *find_command_in_path(char *command, char **envp)
{
    char *path_env = NULL;
    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "PATH=", 5) == 0) {
            path_env = envp[i] + 5;
            break;
        }
    }
    if (path_env == NULL) {
        return NULL;
    }
    char *path = strdup(path_env);
    char *token = strtok(path, ":");
    char full_path[1024];
    while (token != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        if (access(full_path, X_OK) == 0) {
            free(path);
            return strdup(full_path);
        }
        token = strtok(NULL, ":");
    }
    free(path);
    return NULL;
}

void ft_execute(t_cmd_node *cmd_list, char **env)
{
	t_cmd_node *current_node = cmd_list;

	while (current_node != NULL)
	{
		if (is_builtin(current_node->cmd->command))
		{
			builtin_exec(current_node->cmd->args, env);
			return;
		}
		t_cmd *cmd = current_node->cmd;
		char *cmd_path;
		int fd_input = -1, fd_output = -1;
		int pipe_fd[2] = {-1, -1};

		if (cmd->command)
			printf("cmd->command: %s\n", cmd->command);
		// Input redirection
		if (cmd->fd_in)
		{
			fd_input = open(cmd->fd_in, O_RDONLY);
			if (fd_input == -1)
				ft_error("Failed to open input file", 1);
		}
		// Output redirection
		if (cmd->fd_out)
		{
			printf("cmd->fd_out: %s\n", cmd->fd_out);
			fd_output = open(cmd->fd_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_output == -1)
				ft_error("Failed to open output file", 1);
		}
		// Pipe handling
		if (cmd->pipe && pipe(pipe_fd) == -1)
			ft_error("Failed to create pipe", 1);

		cmd->pid = fork();
		if (cmd->pid == 0)
		{ // Child process
			// Input redirection
			if (fd_input != -1)
			{
				if (dup2(fd_input, STDIN_FILENO) == -1)
					ft_error("Failed to redirect stdin", 1);
				close(fd_input);
			}
			// Output redirection
			if (fd_output != -1)
			{
				if (dup2(fd_output, STDOUT_FILENO) == -1)
					ft_error("Failed to redirect stdout", 1);
				close(fd_output);
			}
			// Pipe handling
			if (cmd->pipe)
			{
				close(pipe_fd[0]);
				if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
					ft_error("Failed to redirect stdout", 1);
				close(pipe_fd[1]);
			}
			if (cmd->command[0] == '/')
				cmd_path = cmd->command;
			else
				cmd_path = find_command_in_path(cmd->command, env);
			if (cmd_path) // Check
				printf("cmd_path: %s\n", cmd_path);
			else
				ft_error("Command not found", 127);
			if (execve(cmd_path, current_node->cmd->args, env) == -1)
				ft_error("Command not executable", 126);
		}
		// Parent process
		if (fd_input != -1) close(fd_input);
		if (fd_output != -1) close(fd_output);
		if (cmd->pipe)
		{
			close(pipe_fd[1]);
		}
		waitpid(cmd->pid, &cmd->status, 0);
		current_node = current_node->next;
    }
}
