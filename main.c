/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:59:54 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/26 14:10:14 by dslaveev         ###   ########.fr       */
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
	t_cmd_node *cmd = NULL;

	argv = NULL;
	argc = 0;
	if (argc > 1)
		return (printf("Error: too many arguments\n"), 1);
	prompt = set_prompt("balkanshell$ ");
	while (1)
	{
		input = readline(prompt);
        if (input == NULL || *input == '\0' || is_only_whitespace(input))
		{
			free(input);
			continue;
		}
		init_lexer(&lexer, input);
		parser = init_parser(&lexer);
		parse_command(parser, &cmd, env);
		free_cmd_list(cmd);
		cmd = NULL;
		free(input);
	}
	free(prompt);
}
