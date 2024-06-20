/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:07:17 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/20 12:56:24 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parser	*init_parser(t_lexer *lexer)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (parser == NULL)
		return (NULL);
	parser->lexer = lexer;
	parser->current_token = lexer_get_next_token(lexer);
	return (parser);
}

void	parser_advance(t_parser *parser)
{
	parser->current_token = lexer_get_next_token(parser->lexer);
}

// trying to group together builtin commands before the pipe
// STILL IN PROGRESS
char	**group_together(t_parser *parser)
{
	char	**grouped;
	int		i;

	i = 0;
	grouped = malloc(sizeof(char *) * 1024);
	if (grouped == NULL)
		return (NULL);
	while (parser->current_token != NULL
		&& parser->current_token->type != CHAR_PIPE)
	{
		grouped[i++] = parser->current_token->value;
		parser_advance(parser);
	}
	grouped[i] = NULL;
	return (grouped);
}

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count] != NULL && args)
		count++;
	return (count);
}

void	handle_command(char *command, t_cmd *cmd, t_parser *parser)
{
	if (is_builtin(command))
	{
		cmd->command = ft_strdup(command);
		cmd->args = group_together(parser);
	}
	else
	{
		cmd->command = ft_strdup(command);
		cmd->args = malloc(sizeof(char *) * 2);
		cmd->args[0] = ft_strdup(command);
		cmd->args[1] = NULL;
	}
}

void	handle_argument(char *argument, t_cmd *cmd)
{
	int		count;
	int		i;
	char	**new_args;

	i = 0;
	count = count_args(cmd->args);
	new_args = malloc(sizeof(char *) * (count + 2));
	if (new_args == NULL)
		return ;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(argument);
	new_args[count + 1] = NULL;
	if (cmd->args != NULL)
		free(cmd->args);
	cmd->args = new_args;
}

// Parse Commands and Arguments:

// Modify the parsing function to create a list (or array) of t_cmd structures, each representing a command and its arguments.
// When a pipe (CHAR_PIPE) token is encountered, finish the current command setup and start a new t_cmd structure for the next command.
// Setup Pipes:

// For each pair of commands connected by a pipe, create a pipe using the pipe() system call.
// You will need to keep track of the file descriptors returned by pipe().
// Fork and Execute Commands:

// For each command, fork a new process using fork().
// In the child process:
// If the command is not the first command, redirect its standard input (STDIN_FILENO) to the read end of the previous pipe.
// If the command is not the last command, redirect its standard output (STDOUT_FILENO) to the write end of the current pipe.
// Close unused pipe ends in the child process.
// Execute the command using execve() or a similar function.
// In the parent process, close the pipe ends used by the child process.
// Wait for Child Processes:

// The parent process waits for all child processes to finish using wait() or waitpid().
// or handling, especially for memory allocation and command execution, is minimal and could be improved for robustness.

char	**group_commands_pipe(t_parser *parser)
{
	char	**grouped;
	int		i;

	i = 0;
	grouped = malloc(sizeof(char *) * 1024);
	if (grouped == NULL)
		return (NULL);
	while (parser->current_token != NULL)
	{
		if (parser->current_token->type == CHAR_PIPE)
			parser_advance(parser);
		grouped[i++] = parser->current_token->value;
		parser_advance(parser);
	}
	grouped[i] = NULL;
	return (grouped);
}


void	parse_command(t_parser *parser, t_cmd *cmd , char **env)
{
	int		cmd_flag;
	int	i;

	i = 0;
	cmd_flag = 1;
	cmd = malloc(sizeof(t_cmd));
	// env = NULL;
	if (cmd == NULL)
		return ;
	while (parser->current_token != NULL)
	{
		// printf("parser->current_token->value: %s\n", parser->current_token->value);
		if (parser->current_token->type == CHAR_PIPE)
		{
			cmd_flag = 1;
			// group_commands_pipe(parser);
		}
		if (parser->current_token->type == CHAR_MORE)
		{
			parser_advance(parser);
			cmd->fd_out = parser->current_token->value;
			break;
		}
		if (parser->current_token->type == WORD)
		{
			if (cmd_flag)
			{
				handle_command(parser->current_token->value, cmd, parser);
				cmd_flag = 0;
			}
			else
				handle_argument(parser->current_token->value, cmd);
		}
		parser_advance(parser);
	}
	while (cmd->args[i] != NULL)
	{
		printf("cmd->args[%d]: %s\n", i, cmd->args[i]);
		i++;
	}
	ft_execute(cmd, env);
}

// void	parse_command(t_parser *parser, char **env)
// {
// 	char	*args[1024];
// 	int		i;
// 	int		command;
// 	// char	*expanded_arg;

// 	command = 1;
// 	i = 0;
// 	while (parser->current_token != NULL)
// 	{
// 		if (parser->current_token->type == CHAR_PIPE)
// 		{
// 			command = 1;
// 			//
// 		}
// 		if (parser->current_token->type == WORD && command)
// 		{
// 			// expanded_arg = expander_env(parser->current_token->value, env);
// 			printf("Command: %s\n", parser->current_token->value);
// 			// handle_command(expanded_arg, args, parser, &i);
// 			// args[i++] = parser->current_token->value;
// 			args[i++] = handle_command(parser->current_token->value, args, parser, &i);
// 			command = 0;
// 		}
// 		else if (!command)
// 		{
// 			// expanded_arg = expander_env(parser->current_token->value, env);
// 			args[i++] = handle_argument(parser->current_token->value, args, &i);
// 			printf("argument: %s\n", parser->current_token->value);
// 		}
// 		parser_advance(parser);
// 	}
// 	printf("args[0]: %s\n", args[0]);
// 	if (is_builtin(args[0]))
// 	{
// 		return ;
// 	}
// 	args[i] = NULL;
// 	ft_execute(args, env);
// 	printf("ok\n");
// }

void	parse(t_parser *parser, t_cmd *cmd, char **env)
{
	parse_command(parser, cmd, env);
}
