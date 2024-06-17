/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:26:10 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/14 16:35:43 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	*strip_quotes(char *str)
{
	int	i;

	i = strlen(str);
	if (str[0] == '"' && str[i - 1] == '"')
	{
		str[i - 1] = '\0';
		return (str + 1);
	}
	else if (str[0] == '\'' && str[i - 1] == '\'')
	{
		str[i - 1] = '\0';
		return (str + 1);
	}
	return (str);
}

int	get_token_type(char *value)
{
	if (!strcmp(value, "<"))
		return (CHAR_LESS);
	else if (!strcmp(value, ">"))
		return (CHAR_MORE);
	else if (!strcmp(value, "|"))
		return (CHAR_PIPE);
	else if (!strcmp(value, ";"))
		return (CHAR_SEMICOLON);
	else if (!strcmp(value, " "))
		return (CHAR_SPACE);
	else if (!strcmp(value, "\'"))
		return (CHAR_QUOTE);
	else if (!strcmp(value, "\""))
		return (CHAR_DOUBLE_QUOTE);
	else if (!strcmp(value, "\\"))
		return (CHAR_BACKSLASH);
	else if (!strcmp(value, "$"))
		return (CHAR_DOLLAR);
	else if (!strcmp(value, "&"))
		return (CHAR_AMPERSAND);
	else if (!strcmp(value, "`"))
		return (CHAR_UNDESCP);
	else if (!strcmp(value, "("))
		return (CHAR_BRACKET_O);
	else if (!strcmp(value, ")"))
		return (CHAR_BRACKET_C);
	else
		return (WORD);
}

t_tok	*create_token(char *value, int type)
{
	t_tok	*token;

	token = malloc(sizeof(t_tok));
	if (!token)
		return (NULL);
	token->value = strdup(value);
	token->type = type;
	return (token);
}

void	destroy_token(t_tok *token)
{
	free(token->value);
	free(token);
}

// init_lexer: This function initializes a lexer with a given input string.
// It sets the initial position, current character, and current state.
void	init_lexer(t_lexer *lexer, char *input)
{
	lexer->input = input;
	lexer->cur_char = input[0];
	lexer->pos = 0;
	lexer->cur_state = STATE_GENERAL;
	lexer->token = NULL;
	lexer->token_count = 0;
}

// skip_whitespace: This function advances the lexer's position until
// it finds a non-whitespace character.
void	lexer_skip_whitespace(t_lexer *lexer)
{
	while (lexer->input[lexer->pos] == CHAR_SPACE)
		lexer->pos++;
}


// advance: This function advances the lexer's position by one and updates the current character.
void	lexer_advance(t_lexer *lexer)
{
	lexer->pos++;
	if ((unsigned int)lexer->pos < strlen(lexer->input))
		lexer->cur_char = lexer->input[lexer->pos];
	else
		lexer->cur_char = '\0';
}

// lex_number: This function lexes a number
// from the input string.
t_tok	*lexer_number(t_lexer *lexer)
{
	int		start_pos;
	char	*nbr;
	t_tok	*token;

	start_pos = lexer->pos;
	while (lexer->cur_char >= '0' && lexer->cur_char <= '9')
		lexer_advance(lexer);
	nbr = strndup(lexer->input + start_pos, lexer->pos - start_pos);
	token = create_token(nbr, NUMBER);
	return (token);
}

int	is_string_identify(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || ((c >= '0' && c <= '9') || c == '_'));
}
// lex_identifier: This function lexes an
// identifier from the input string.
t_tok	*lexer_identifier(t_lexer *lexer)
{
	int		start_pos;
	char	*ident;
	t_tok	*token;

	start_pos = lexer->pos;
	while (is_string_identify(lexer->cur_char) || lexer->cur_char == '.' || lexer->cur_char == '\'')
		lexer_advance(lexer);
	ident = strndup(lexer->input + start_pos, lexer->pos - start_pos);
	token = create_token(ident, WORD);
	return (token);
}

int	is_pipe(char c)
{
	return (c == CHAR_PIPE);
}

t_tok	*lexer_pipe(t_lexer *lexer)
{
	t_tok	*token;

	token = create_token("|", CHAR_PIPE);
	lexer_advance(lexer);
	return (token);
}

int	is_quote(char c)
{
	return (c == CHAR_QUOTE);
}

t_tok	*lexer_quote(t_lexer *lexer)
{
	t_tok	*token;

	token = create_token("\'", CHAR_QUOTE);
	lexer_advance(lexer);
	return (token);
}

int is_double_quote(char c)
{
	return (c == CHAR_DOUBLE_QUOTE);
}

t_tok	*lexer_double_quote(t_lexer *lexer)
{
	t_tok	*token;

	token = create_token("\"", CHAR_DOUBLE_QUOTE);
	lexer_advance(lexer);
	return (token);
}

int	is_redirection(char c)
{
	return (c == CHAR_LESS || c == CHAR_MORE);
}

t_tok	*lexer_redirection(t_lexer *lexer)
{
	t_tok	*token;

	token = create_token(&lexer->cur_char, get_token_type(&lexer->cur_char));
	lexer_advance(lexer);
	return (token);
}

int	is_with_dash(char c, char next_c)
{
	return (c == '-' && ((next_c >= 'a' && next_c <= 'z') || (next_c >= 'A' && next_c <= 'Z')));
}

t_tok	*lexer_dash(t_lexer *lexer)
{
	t_tok	*token;
	int		start_pos;
	char	*ident;

	start_pos = lexer->pos;
	lexer_advance(lexer);
	while ((lexer->cur_char >= 'a' && lexer->cur_char <= 'z') || (lexer->cur_char >= 'A' && lexer->cur_char <= 'Z') || lexer->cur_char == '-')
		lexer_advance(lexer);
	ident = strndup(lexer->input + start_pos, lexer->pos - start_pos);
	token = create_token(ident, WORD);
	return (token);
}

t_tok	*lexer_token_name(t_lexer *lexer)
{
	int 	start_pos;
	char	*word;
	t_tok	*token;

	start_pos = lexer->pos;
	while (lexer->cur_char != CHAR_SPACE && lexer->cur_char != '\0')
		lexer_advance(lexer);
	word = strndup(lexer->input + start_pos, lexer->pos - start_pos);
	token = create_token(word, WORD);
	return (token);
}

t_tok	*process_token(t_lexer *lexer, char end_char, int token_type, int quotes)
{
	int		start_pos;
	char	*ident;
	t_tok	*token;

	lexer_advance(lexer);
	start_pos = lexer->pos;
	while (lexer->cur_char != end_char && lexer->cur_char != '\0')
		lexer_advance(lexer);
	ident = strndup(lexer->input + start_pos, lexer->pos - start_pos);
	if (quotes)
		ident = strip_quotes(ident);
	token = create_token(ident, token_type);
	lexer_advance(lexer);
	return (token);
}


// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// -----------------------------------------------------------------------------

// next_token: This function returns the next token from the input string.
// It uses the other lexing functions to recognize the type of the next token and lex it.
t_tok *lexer_get_next_token(t_lexer *lexer)
{
	t_tok *token = NULL;

	while (lexer->cur_char != '\0')
	{
		switch (lexer->cur_char)
		{
			case CHAR_SPACE:
				lexer_advance(lexer);
				break;
			case CHAR_LESS:
				token = create_token("CHAR_LESS", CHAR_LESS);
				lexer_advance(lexer);
				return (token);
			case CHAR_MORE:
				if (lexer->input[lexer->pos + 1] == CHAR_MORE)
				{
					token = create_token("CHAR_DOUBLE_MORE", CHAR_MORE);
					lexer_advance(lexer);
				}
				else
					token = create_token("CHAR_MORE", CHAR_MORE);
				lexer_advance(lexer);
				return (token);
			case CHAR_PIPE:
				return ((token = lexer_pipe(lexer), token));
			case '(':
				return process_token(lexer, '(', COMMAND_GROUP, 0);
			case CHAR_QUOTE:
				return process_token(lexer, CHAR_QUOTE, COMMAND_GROUP, 1);
			case CHAR_DOUBLE_QUOTE:
				return process_token(lexer, CHAR_DOUBLE_QUOTE, COMMAND_GROUP, 1);
			default:
				if (is_string_identify(lexer->cur_char))
					return ((token = lexer_identifier(lexer)), token);
				else if (is_with_dash(lexer->cur_char, lexer->input[lexer->pos + 1]))
					return ((token = lexer_dash(lexer)), token);
				else if (lexer->cur_char != CHAR_SPACE)
					return ((token = lexer_token_name(lexer)), token);
				else
					lexer_advance(lexer);
				break;
			}
	}
	return (NULL);
}

// if somethings wrong with tokens us this

// t_tok	*lexer_get_next_token(t_lexer *lexer)
// {
// 	t_tok	*token;

// 	token = NULL;
// 	while (lexer->cur_char != '\0')
// 	{
// 		if (lexer->cur_char == CHAR_SPACE)
// 			lexer_advance(lexer);
// 		else if (lexer->cur_char == CHAR_LESS)
// 		{
// 			token = create_token("CHAR_LESS", CHAR_LESS);
// 			lexer_advance(lexer);
// 			return (token);
// 		}
// 		else if (lexer->cur_char == CHAR_MORE)
// 		{
// 			if (lexer->input[lexer->pos + 1] == CHAR_MORE)
// 			{
// 				token = create_token("CHAR_DOUBLE_MORE", CHAR_MORE);
// 				lexer_advance(lexer);
// 			}
// 			else
// 				token = create_token("CHAR_MORE", CHAR_MORE);
// 			lexer_advance(lexer);
// 			return (token);
// 		}
// 		else if (lexer->cur_char == CHAR_PIPE)
// 		{
// 			token = create_token("CHAR_PIPE", CHAR_PIPE);
// 			lexer_advance(lexer);
// 			return (token);
// 		}
// 		else if (lexer->cur_char == '(')
// 			return (process_token(lexer, '(', COMMAND_GROUP, 0));
// 		else if (lexer->cur_char == CHAR_QUOTE)
// 			return (process_token(lexer, CHAR_QUOTE, COMMAND_GROUP, 1));
// 		else if (lexer->cur_char == CHAR_DOUBLE_QUOTE)
// 			return (process_token(lexer, CHAR_DOUBLE_QUOTE, COMMAND_GROUP, 1));
// 		else if (is_string_identify(lexer->cur_char))
// 		{
// 			token = lexer_identifier(lexer);
// 			return (token);
// 		}
// 		else if (is_with_dash(lexer->cur_char, lexer->input[lexer->pos + 1]))
// 		{
// 			token = lexer_dash(lexer);
// 			return (token);
// 		}
// 		else if (lexer->cur_char != CHAR_SPACE)
// 		{
// 			token = lexer_token_name(lexer);
// 			return (token);
// 		}
// 		else
// 		{
// 			lexer_advance(lexer);
// 			continue;
// 		}
// 	}
// 	return (0);
// }

// << < > >> | ' '
