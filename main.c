/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:59:54 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/20 12:42:24 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

extern t_sig	g_sig;

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




// char	*find_cmmand_in_path(const char *command, char **env)
// {
// 	const char	*path_env;
// 	char		*path;
// 	char		*token;
// 	char		*full_path;

// 	if (ft_strchr(command, '/') != NULL)
// 		return (ft_strdup(command));
// 	path_env = getenv("PATH");
// 	if (path_env == NULL)
// 		return (NULL);
// 	path = ft_strdup(path_env);
// 	token = strtok(path, ":");
// 	while (token != NULL)
// 	{
// 		full_path = malloc(ft_strlen(token) + ft_strlen(command) + 2);
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			free(path);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		token = strtok(NULL, ":");
// 	}
// 	free(path);
// 	return (NULL);
// }

// char *execute_command_and_capture_output(char *command) {
//     int pipefd[2];
//     pid_t pid;
//     char buffer[1024];
//     int status;
//     size_t read_bytes;

//     // Create a pipe to capture stdout
//     if (pipe(pipefd) == -1) {
//         perror("pipe");
//         return NULL;
//     }

//     pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         return NULL;
//     } else if (pid == 0) {
//         // Child process
//         close(pipefd[0]); // Close unused read end
//         dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
//         execlp("sh", "sh", "-c", command, (char *)NULL);
//         // If execlp returns, it must have failed
//         perror("execlp");
//         exit(EXIT_FAILURE);
//     } else {
//         // Parent process
//         close(pipefd[1]); // Close unused write end
//         waitpid(pid, &status, 0); // Wait for child process to finish

//         read_bytes = read(pipefd[0], buffer, sizeof(buffer) - 1);
//         if (read_bytes > 0) {
//             buffer[read_bytes] = '\0'; // Null-terminate the string
//         } else {
//             buffer[0] = '\0'; // Empty output
//         }
//         close(pipefd[0]);
//     }

//     return strdup(buffer); // Caller is responsible for freeing this memory
// }

char	*expander_env(char *arg, char **env)
{
	char	*env_var;

	env = NULL;
	if (arg[0] == '$')
	{
		if (arg[1] == '?')
		{
			printf("exit status\n");
			// return (ft_itoa(g_sig.exstatus));
		}
		else
		{
			env_var = getenv(arg + 1);
			if (env_var != NULL)
				return (ft_strdup(env_var));
		}
	}
	return (ft_strdup(arg));
}

// void    ft_execute(t_cmd *cmd, char **env) {
//     pid_t   pid;
//     int     status;
//     int     fd_in, fd_out;

//     // Handle input redirection
//     if (cmd->input_file) {
//         fd_in = open(cmd->input_file, O_RDONLY);
//         if (fd_in < 0) ft_error("Failed to open input file", 1);
//         dup2(fd_in, STDIN_FILENO);
//         close(fd_in);
//     }

//     // Handle output redirection
//     if (cmd->output_file) {
//         fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0666);
//         if (fd_out < 0) ft_error("Failed to open output file", 1);
//         dup2(fd_out, STDOUT_FILENO);
//         close(fd_out);
//     }

//     // Handle pipes and execute commands recursively if there's a pipeline
//     if (cmd->next) {
//         pipe(cmd->pipe);
//         pid = fork();
//         if (pid == 0) { // Child
//             close(cmd->pipe[0]); // Close unused read end
//             dup2(cmd->pipe[1], STDOUT_FILENO); // Redirect stdout to pipe write end
//             execve(cmd->command, cmd->args, env);
//             ft_error("Command not executable", 126);
//         } else { // Parent
//             close(cmd->pipe[1]); // Close unused write end
//             dup2(cmd->pipe[0], STDIN_FILENO); // Redirect stdin to pipe read end
//             ft_execute(cmd->next, env); // Recursively handle the next command in the pipeline
//         }
//     } else {
//         execve(cmd->command, cmd->args, env);
//     }
// }

// working
// void	ft_execute(t_cmd *cmd, char **env)
// {
// 	pid_t	pid;
// 	int		status;
// 	char	*cmd_path;
// 	int		fd_input, fd_output;

// 	if (cmd->fd_in)
// 	{
// 		fd_input = open(cmd->fd_in, O_RDONLY);
// 		if (fd_input == -1)
// 			ft_error("Failed to open input file", 1);
// 		dup2(fd_input, STDIN_FILENO);
// 		close(fd_input);
// 	}
// 	if (cmd->fd_out)
// 	{
// 		fd_output = open(cmd->fd_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (fd_output == -1)
// 			ft_error("Failed to open output file", 1);
// 		dup2(fd_output, STDOUT_FILENO);
// 		close(fd_output);
// 	}
// 	printf("cmd->command: %s\n", cmd->command);
// 	if (is_builtin(cmd->command))
// 	{
// 		builtin_exec(cmd->args, env);
// 		return ;
// 	}
// 	cmd_path = get_cmd_path(cmd->command, env);
// 	if (!cmd_path)
// 		ft_error("Command not found", 127);
// 	pid = fork();
// 	if (pid == -1)
// 		ft_error("Failed to fork", 1);
// 	else if (pid == 0)
// 	{
// 		if (execve(cmd_path, cmd->args, env) == -1)
// 			ft_error("Command not executable", 126);
// 		}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 	}
// }

int main(int argc, char **argv, char **env)
{
	char		*prompt;
	char		*input;
	t_lexer		lexer;
	t_parser	*parser;
	t_cmd		*cmd;

	argv = NULL;
	cmd = NULL;
	if (argc > 1)
		return (printf("Error: too many arguments\n"), 1);
	prompt = set_prompt("balkanshell$ ");
	while (1)
	{
		input = readline(prompt);
		if (input == NULL)
			break ;
		init_lexer(&lexer, input);
		parser = init_parser(&lexer);
		parse(parser, cmd, env);
	}
	free(prompt);
}

// void ft_execute(t_cmd *cmd, char **env) {
//     char *cmd_path;
//     int fd_input, fd_output;
//     int pipe_fd[2]; // Array to hold the file descriptors for the pipe

//     // Check and handle input redirection
//     if (cmd->fd_in) {
//         fd_input = open(cmd->fd_in, O_RDONLY);
//         if (fd_input == -1)
//             ft_error("Failed to open input file", 1);
//         if (dup2(fd_input, STDIN_FILENO) == -1)
//             ft_error("Failed to redirect stdin", 1);
//         close(fd_input);
//     }

//     // Create a pipe if there's a next command to pipe to
//     if (cmd->next) {
//         if (pipe(pipe_fd) == -1)
//             ft_error("Failed to create pipe", 1);
//     }

//     cmd->pid = fork();
//     if (cmd->pid == -1)
//         ft_error("Failed to fork", 1);
//     else if (cmd->pid == 0) { // Child process
//         // If there's a next command, redirect STDOUT to the pipe's write end
//         if (cmd->next) {
//             close(pipe_fd[0]); // Close the read end; not needed in this child
//             if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
//                 ft_error("Failed to redirect stdout to pipe", 1);
//             close(pipe_fd[1]); // Close the write end after duplicating
//         }

//         // Execute the command
//         cmd_path = get_cmd_path(cmd->command, env);
//         if (!cmd_path)
//             ft_error("Command not found", 127);
//         if (execve(cmd_path, cmd->args, env) == -1)
//             ft_error("Command not executable", 126);
//     } else { // Parent process
//         if (cmd->next) {
//             int status;
//             close(pipe_fd[1]); // Close the write end; not needed in the parent

//             // Wait for the first child to finish
//             waitpid(cmd->pid, &status, 0);

//             // Fork a new process for the next command
//             pid_t pid_next = fork();
//             if (pid_next == 0) {
//                 // Redirect STDIN to the pipe's read end
//                 if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
//                     ft_error("Failed to redirect stdin from pipe", 1);
//                 close(pipe_fd[0]); // Close the read end after duplicating

//                 // Execute the next command (assuming next command setup is similar)
//                 // This part needs to be adapted based on how you manage commands
//             } else {
//                 close(pipe_fd[0]); // Close the read end in the parent
//                 // Wait for the second child to finish
//                 waitpid(pid_next, &status, 0);
//             }
//         }
//     }
// }
