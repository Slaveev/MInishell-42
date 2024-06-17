/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:07:17 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/17 12:49:06 by dslaveev         ###   ########.fr       */
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

void	handle_command(char *command, char **args, t_parser *parser, int *i)
{
	char	**grouped;
	char	**env;

	env = NULL;
	if (is_builtin(command))
	{
		grouped = group_together(parser);
		builtin_exec(grouped, env);
		free(grouped);
	}
	else
	{
		args[(*i)++] = command;
	}
}

void	handle_argument(char *arg, char **args, int *i)
{
	args[(*i)++] = arg;
}

// int	reset_command(t_parser *parser, int command)
// {
// 	command = 0;
// 	if (parser->current_token->type == CHAR_PIPE)
// 	{
// 		command = 1;
// 	}
// 	return (command);
// }

char	*expander_env(char *arg, char **env)
{
	char	*env_var;

	env = NULL;
	if (arg[0] == '$')
	{
		env_var = getenv(arg + 1);
		if (env_var != NULL)
			return (ft_strdup(env_var));
	}
	return (ft_strdup(arg));
}

char	**expander_argv(char **argv, char **env)
{
	int		i;
	char	**expanded_argv;

	i = 0;
	while (argv[i] != NULL)
		i++;
	expanded_argv = malloc((i + 1) * sizeof(char *));
	i = 0;
	while (argv[i] != NULL)
	{
		expanded_argv[i] = expander_env(argv[i], env);
		i++;
	}
	expanded_argv[i] = NULL;
	return (expanded_argv);
}

void	parse_command(t_parser *parser, char **env)
{
	char	*args[1024];
	int		i;
	int		command;
	char	*expanded_arg;

	command = 1;
	i = 0;
	while (parser->current_token != NULL)
	{
		if (parser->current_token->type == CHAR_PIPE)
		{
			command = 1;
			//
		}
		if (parser->current_token->type == WORD && command)
		{
			expanded_arg = expander_env(parser->current_token->value, env);
			printf("Command: %s\n", parser->current_token->value);
			handle_command(expanded_arg, args, parser, &i);
			// handle_command(parser->current_token->value, args, parser, &i);
			command = 0;
		}
		else if (!command)
		{
			expanded_arg = expander_env(parser->current_token->value, env);
			handle_argument(parser->current_token->value, args, &i);
			printf("argument: %s\n", parser->current_token->value);
		}
		parser_advance(parser);
	}
	args[i] = NULL;
	ft_execute(args, env);
	printf("ok\n");
}

void	parse(t_parser *parser, char **env)
{
	parse_command(parser, env);
}
