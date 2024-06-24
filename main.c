/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:59:54 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/24 12:27:12 by dslaveev         ###   ########.fr       */
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

int is_only_whitespace(const char *str)
{
	while (*str) {
		if (!isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

int main(int argc, char **argv, char **env) {
    char *prompt;
    char *input;
    t_lexer lexer;
    t_parser *parser;
    t_cmd_node *cmd = NULL; // Change to a pointer to t_cmd_node, initialized to NULL.

	argv = NULL;
	argc = 0;
    if (argc > 1)
        return (printf("Error: too many arguments\n"), 1);
    prompt = set_prompt("balkanshell$ ");
    while (1) {
        input = readline(prompt);
		// for empty input
        if (input == NULL || *input == '\0' || is_only_whitespace(input))
		{
			free(input);
			continue;
		}
        init_lexer(&lexer, input);
        printf("dan\n");
        parser = init_parser(&lexer);
        printf("parser: %s\n", parser->current_token->value);
        parse_command(parser, &cmd, env);
        // free cmd because it remembers the last command
		free_cmd_list(cmd);
		cmd = NULL;
		free(input);
    }
    free(prompt);
}

// int main(int argc, char *argv[], char *envp[]) {

// 	argc = 0;
// 	argv = NULL;
//     char *command;
//     while (1) {
//         command = readline("Shell> ");
//         if (!command) {
//             break;
//         }
//         if (strlen(command) > 0) {
//             add_history(command);
//         }
//         char *commands[1024];
//         int num_pipes = 0;
//         char *token;
//         token = strtok(command, "|");
//         while (token != NULL) {
//             commands[num_pipes++] = token;
//             token = strtok(NULL, "|");
//         }
//         commands[num_pipes] = NULL;
//         int pipefds[2 * (num_pipes - 1)];
//         pid_t pid;
//         int status;
//         int i;
//         for (i = 0; i < num_pipes - 1; i++) {
//             if (pipe(pipefds + i * 2) == -1) {
//                 perror("pipe");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         for (i = 0; i < num_pipes; i++) {
//             pid = fork();
//             if (pid == -1) {
//                 perror("fork");
//                 exit(EXIT_FAILURE);
//             } else if (pid == 0) {
//                 if (i > 0) {
//                     if (dup2(pipefds[(i - 1) * 2], 0) == -1) {
//                         perror("dup2");
//                         exit(EXIT_FAILURE);
//                     }
//                 }
//                 if (i < num_pipes - 1) {
//                     if (dup2(pipefds[i * 2 + 1], 1) == -1) {
//                         perror("dup2");
//                         exit(EXIT_FAILURE);
//                     }
//                 }
//                 for (int j = 0; j < 2 * (num_pipes - 1); j++) {
//                     close(pipefds[j]);
//                 }
//                 t_cmd cmd;
//                 t_parser parser;
//                 parser.current_token->value = token; // Set the initial token for the parser
//                 printf("Token: %s\n", parser.current_token->value);
// 				parse_command(&parser, &cmd, envp);
//                 exit(EXIT_SUCCESS);
//             }
//         }
//         for (i = 0; i < 2 * (num_pipes - 1); i++) {
//             close(pipefds[i]);
//         }
//         for (i = 0; i < num_pipes; i++) {
//             wait(&status);
//         }
//         free(command);
//     }
//     return 0;
// }
