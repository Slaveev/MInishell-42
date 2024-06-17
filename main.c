/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:59:54 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/17 12:47:39 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_token(t_tok *token)
{
	printf("type: %d, value: %s\n", token->type, token->value);
}

char	*set_prompt(char *input)
{
	char	*prompt;

	if (input == NULL)
		return (NULL);
	prompt = malloc(strlen(input) + 1);
	if (prompt == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	strcpy(prompt, input);
	return (prompt);
}


// void	execute_command_with_pipe(char *cmd1[], char *cmd2[], int fd)
// {
// 	int		pfd[2];
// 	pid_t	pid1;
// 	pid_t	pid2;

// 	if (pipe(pfd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	pid1 = fork();
// 	if (pid1 == 0)
// 	{
// 		dup2(pfd[1], STDOUT_FILENO);
// 		close(pfd[0]);
// 		close(pfd[1]);
// 		if (execvp(cmd1[0], cmd1) == -1)
// 		{
// 			perror("execvp");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	pid2 = fork();
// 	if (pid2 == 0)
// 	{
// 		dup2(fd, STDOUT_FILENO);
// 	}
// 	dup2(pfd[0], STDIN_FILENO);
// 	close(pfd[0]);
// 	close(pfd[1]);
// 	if (execvp(cmd2[0], cmd2) == -1)
// 	{
// 		perror("execvp");
// 		exit(EXIT_FAILURE);
// 	}
// 	close(pfd[0]);
// 	close(pfd[1]);
// 	waitpid(pid1, NULL, 0);
// 	waitpid(pid2, NULL, 0);
// }

// void	execute_command(char *command, char **args, int out_fd)
// {
// 	pid_t	pid;
// 	int		status;
// 	int		pipex_index = -1;
// 	int i = 0;

// 	while (args[i] != NULL)
// 	{
// 		if (strcmp(args[i], "|") == 0)
// 			pipex_index = i;
// 		i++;
// 	}
// 	if (pipex_index != -1)
// 	{
// 		args[pipex_index] = NULL;
// 		char **cmd1_args = args;
// 		char **cmd2_args = &args[pipex_index + 1];
// 		execute_command_with_pipe(cmd1_args, cmd2_args, out_fd);
// 	}
// 	else
// 	{
// 		if (is_builtin(command))
// 			return ;
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			if (out_fd != -1)
// 			{
// 				dup2(out_fd, STDOUT_FILENO);
// 				close(out_fd);
// 			}
// 			if (execvp(command, args) == -1)
// 				perror("not ok");
// 			exit(EXIT_FAILURE);
// 		}
// 		else if (pid < 0)
// 			perror("balkanshell");
// 		else
// 		{
// 			do
// 			{
// 				waitpid(pid, &status, WUNTRACED);
// 			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
// 		}
// 	}
// }


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



void	ft_execute(char **argv, char **envp)
{
	pid_t	pid;
	char	*cmd_path;
	int		status;

	if (is_builtin(argv[0]))
	{
		builtin_exec(argv, envp);
		return ;
	}
	cmd_path = NULL;
	pid = fork();
	if (pid == -1)
		ft_error("Failed to fork", 1);
	else if (pid == 0)
	{
		printf("fml\n");
		if (execve(argv[0], argv, envp) == -1)
		{
			// dup here
			cmd_path = get_cmd_path(argv[0], envp);
			printf("cmd path: %s\n", cmd_path);
			if (!cmd_path)
			{
				// free_str_array(cmd);
				ft_error("Command not found", 127);
			}
			if (execve(cmd_path, argv, envp) == -1)
			{
				free(cmd_path);
				// free_str_array(cmd);
				ft_error("Command not executable", 126);
			}
			free(cmd_path);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

void	child_process(char **args, int *pfd, char **env)
{
	int		fdin;

	if (access(args[1], F_OK) == -1)
		ft_error("No such file", 1);
	fdin = open(args[1], O_RDONLY);
	if (fdin <= -1)
		ft_error("Error opening input file", EXIT_FAILURE);
	if (dup2(fdin, STDIN_FILENO) < 0)
	{
		perror("Dup2 failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(pfd[1], STDOUT_FILENO) < 0)
	{
		perror("Dup2 failed");
		exit(EXIT_FAILURE);
	}
	close(fdin);
	ft_close_fd(pfd);
	ft_execute(args, env);
	exit(EXIT_FAILURE);
}

void	parent_process(char **args, int *pfd, char **env)
{
	int		fdout;

	fdout = open(args[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fdout <= -1)
	{
		perror("Error openning file descriptor");
		exit(EXIT_FAILURE);
	}
	if (dup2(fdout, STDOUT_FILENO) < 0)
	{
		perror("Dup2 failed");
		exit(127);
	}
	if (dup2(pfd[0], STDIN_FILENO) < 0)
	{
		perror("Dup2 failed");
		exit(127);
	}
	close(fdout);
	ft_close_fd(pfd);
	ft_execute(args, env);
	exit(EXIT_FAILURE);
}

// int	pid_process(char **args, int *pfd, char **env)
// {
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		status1;

// 	if (pipe(pfd) == -1)
// 		ft_error("Failed to create pipe", EXIT_FAILURE);
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		ft_error("Failed to fork child process 1", EXIT_FAILURE);
// 	if (pid1 == 0)
// 		child_process(args, pfd, env);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		ft_error("Failed to fork child process 2", EXIT_FAILURE);
// 	if (pid2 == 0)
// 		parent_process(args, pfd, env);
// 	ft_close_fd(pfd);
// 	waitpid(pid2, &status1, 0);
// 	if (WIFEXITED(status1))
// 		status1 = WEXITSTATUS(status1);
// 	return (status1);
// }

int main(int argc, char **argv, char **env)
{
	char		*prompt;
	char		*input;
	t_lexer		lexer;
	t_parser	*parser;
	// int			status;
	// int			pfd[2];

	// env = NULL;
	argv = NULL;
	if (argc > 1)
		return (printf("Error: too many arguments\n"), 1);
	prompt = set_prompt("balkanshell$ ");
	while (1)
	{
		input = readline(prompt);
		init_lexer(&lexer, input);
		parser = init_parser(&lexer);
		parse(parser, env);
		// status = pid_process(parser->input, pfd, env);
	}
	free(prompt);
}
// need two global var
// nextToken
// resultTree
// how it should work

// 1 scanToken()
// 2 resultTree = parseE()
// 3 if nextToken != EOF
// 4 error()
// 5 printTree(resultTree)

// make while loop to first check if its a builtin
	// if it is execute builtin
// else its not a builtin so we need to fork and run the command
	// exec the command

// parseexec - redir { aaa redir }+
// parseredirs - { > < >> aaa}
// pipe - exec [| pipe]
// parseline - pipe {&} [; line]
// block - (line) redir

// pipe -> exec
// pipe -> exec | pipe

// aaa || (bbb | (ccc | ddd))
			// pipe
		  //    |
		// exec	  pipe
		//  aaa    |
	//       exec     pipe
	//       bbb       |
	//  		  exec     pipe
			//	  ccc       |
						// exec
						// ddd

// first one should be a command
// after the first one should be an argument
// if its a pipe then we are expecting a command
