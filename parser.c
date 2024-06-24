/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:07:17 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/24 12:10:04 by dslaveev         ###   ########.fr       */
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

// char	**group_commands_pipe(t_parser *parser)
// {
// 	char	**grouped;
// 	int		i;

// 	i = 0;
// 	grouped = malloc(sizeof(char *) * 1024);
// 	if (grouped == NULL)
// 		return (NULL);
// 	while (parser->current_token != NULL)
// 	{
// 		if (parser->current_token->type == CHAR_PIPE)
// 			parser_advance(parser);
// 		grouped[i++] = parser->current_token->value;
// 		printf("grouped[%d]: %s\n", i, grouped[i]);
// 		parser_advance(parser);
// 	}
// 	grouped[i] = NULL;
// 	return (grouped);
// }

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
		// printf("Command: %s\n", command);
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
	// printf("Argument: %s\n", argument);
	new_args[count + 1] = NULL;
	if (cmd->args != NULL)
		free(cmd->args);
	cmd->args = new_args;
}

// void	arguments_init(t_cmd *cmd)
// {
// 	t_cmd	*new_cmd;

// 	new_cmd = malloc(sizeof(t_cmd));
// 	if (new_cmd == NULL)
// 		return ;
// 	new_cmd->args = NULL;
// 	new_cmd->next = NULL;
// }

void print_command_list(t_cmd *head)
{
    t_cmd *current = head;
    while (current != NULL)
    {
        printf("Command: %s\n", current->command);

        // Print the arguments
        if (current->args != NULL)
        {
            printf("Arguments: ");
            for (char **arg = current->args; *arg != NULL; ++arg)
            {
                printf("%s ", *arg);
            }
            printf("\n");
        }
        else
        {
            printf("Arguments: None\n");
        }

        current = current->next;

        // Print a separator between commands
        printf("-----\n");
    }
}

// WORKING ------- WITH REDIR
// void	parse_command(t_parser *parser, t_cmd *cmd , char **env)
// {
// 	int		cmd_flag;
// 	int	i;

// 	i = 0;
// 	cmd_flag = 1;
// 	cmd = malloc(sizeof(t_cmd));
// 	// env = NULL;
// 	if (cmd == NULL)
// 		return ;
// 	while (parser->current_token != NULL)
// 	{
// 		// printf("parser->current_token->value: %s\n", parser->current_token->value);
// 		if (parser->current_token->type == CHAR_PIPE)
// 		{
// 			cmd_flag = 1;
// 			cmd->pipe = true;
// 			parser_advance(parser);
// 		}
// 		if (parser->current_token->type == CHAR_MORE)
// 		{
// 			cmd_flag = 0;
// 			parser_advance(parser);
// 			cmd->fd_out = parser->current_token->value;
// 			break;
// 		}
// 		if (parser->current_token->type == WORD)
// 		{
// 			if (cmd_flag)
// 			{
// 				handle_command(parser->current_token->value, cmd, parser);
// 				cmd_flag = 0;
// 				// printf("cmd->command: %s\n", cmd->command);
// 			}
// 			else
// 				handle_argument(parser->current_token->value, cmd);
// 		}
// 		parser_advance(parser);
// 		print_command_list(cmd);
// 	}
// 	ft_execute(cmd, env);
// }

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

void free_cmd_list(t_cmd_node *cmd_list) {
    t_cmd_node *tmp;
    while (cmd_list != NULL) {
        tmp = cmd_list;
        cmd_list = cmd_list->next;
        free(tmp->cmd->command);
        // Free arguments if allocated
        // for (int i = 0; tmp->cmd->args[i] != NULL; i++) {
        //     free(tmp->cmd->args[i]);
        // }
        free(tmp->cmd->args);
        free(tmp->cmd);
        free(tmp);
    }
}

void parse_command(t_parser *parser, t_cmd_node **cmd_list, char **env) {
    int cmd_flag = 1;
    t_cmd_node *current_node = NULL;
    t_cmd *current_cmd = NULL;

    if (!parser || !cmd_list) {
        perror("parser or cmd_list is NULL");
        return;
    }
    printf("cmd_list: %p\n", *cmd_list);
    while (parser->current_token != NULL) {
        if (cmd_flag == 1) {
            t_cmd_node *new_node = calloc(1, sizeof(t_cmd_node));
            if (new_node == NULL) {
                perror("Failed to allocate memory for cmd_node");
                free_cmd_list(*cmd_list);
                return;
            }
            if (*cmd_list == NULL) {
                *cmd_list = new_node;
            } else {
                if (current_node != NULL) {
                    current_node->next = new_node;
                }
            }
            current_node = new_node;
            current_node->cmd = calloc(1, sizeof(t_cmd));
            if (current_node->cmd == NULL) {
                perror("Failed to allocate memory for cmd");
                free_cmd_list(*cmd_list);
                return;
            }
            current_cmd = current_node->cmd;
            cmd_flag = 0; // Reset cmd_flag as we're now expecting arguments for this command
        }

        if (parser->current_token->type == CHAR_PIPE) {
            current_cmd->pipe = true; // Set the pipe flag
            cmd_flag = 1; // Next token should be a new command
        } else if (parser->current_token->type == WORD) {
            if (current_cmd->command == NULL) {
                current_cmd->command = strdup(parser->current_token->value);
                if (current_cmd->command == NULL) {
                    perror("Failed to duplicate command string");
                    free_cmd_list(*cmd_list);
                    return;
                }
                current_cmd->args = calloc(2, sizeof(char*)); // Space for command and NULL terminator
                if (current_cmd->args == NULL) {
                    perror("Failed to allocate memory for args");
                    free_cmd_list(*cmd_list);
                    return;
                }
                current_cmd->args[0] = current_cmd->command; // First arg is the command itself
            } else {
				// Find the current length of args
				int args_len;
				for (args_len = 0; current_cmd->args[args_len] != NULL; ++args_len);

				// Reallocate args to accommodate the new argument and the NULL terminator
				char **new_args = realloc(current_cmd->args, sizeof(char*) * (args_len + 2));
				if (new_args == NULL) {
					perror("Failed to reallocate memory for args");
					free_cmd_list(*cmd_list);
					return;
				}
				current_cmd->args = new_args;
				current_cmd->args[args_len] = strdup(parser->current_token->value);
				if (current_cmd->args[args_len] == NULL) {
					perror("Failed to duplicate argument string");
					free_cmd_list(*cmd_list);
					return;
				}
				// NULL terminate the args array
				current_cmd->args[args_len + 1] = NULL;
			}
        }

        parser_advance(parser);
    }
	printf("cmd_list: %p\n", *cmd_list);
	printf("cmd_list->cmd->command: %s\n", (*cmd_list)->cmd->command);
	printf("cmd_list->cmd->args: %s\n", (*cmd_list)->cmd->args[0]);
	printf("cmd_list->cmd->args: %s\n", (*cmd_list)->cmd->args[1]);
	ft_execute(*cmd_list, env);
}


// ALSO WORKING
// void parse_command(t_parser *parser, t_cmd_node **cmd_list, char **env) {
//     int cmd_flag = 1;
//     t_cmd_node *current_node = NULL;
//     t_cmd *current_cmd = NULL;

//     // Initialize cmd_list to NULL to avoid dangling pointer issues
//     // *cmd_list = NULL;
// 	// env = NULL;

// 	if (!parser || !cmd_list) {
//     perror("parser or cmd_list is NULL");
//     return;
// 	}
// 	printf("cmd_list: %p\n", *cmd_list);
//     while (parser->current_token != NULL) {
//         if (cmd_flag == 1) {
//             t_cmd_node *new_node = calloc(1, sizeof(t_cmd_node));
//             if (new_node == NULL) {
//                 perror("Failed to allocate memory for cmd_node");
//                 free_cmd_list(*cmd_list);
//                 return;
//             }
//             if (*cmd_list == NULL) {
//                 *cmd_list = new_node;
//             } else {
//                 if (current_node != NULL) {
//                     current_node->next = new_node;
//                 }
//             }
// 			printf("ok\n");
//             current_node = new_node;
//             current_node->cmd = calloc(1, sizeof(t_cmd));
//             if (current_node->cmd == NULL) {
//                 perror("Failed to allocate memory for cmd");
//                 free_cmd_list(*cmd_list);
//                 return;
//             }
//             current_cmd = current_node->cmd;
//             cmd_flag = 0;
//         }

//         if (parser->current_token->type == CHAR_PIPE) {
//             current_cmd->pipe = true; // Set the pipe flag
//             cmd_flag = 1;
//         } else if (parser->current_token->type == WORD) {
//             if (current_cmd->command == NULL) {
// 				current_cmd->command = strdup(parser->current_token->value);
// 				if (current_cmd->command == NULL) {
// 					perror("Failed to duplicate command string");
// 					free_cmd_list(*cmd_list);
// 					return;
// 				}
// 				// Initialize args array with space for 2 pointers: one argument and one NULL terminator
// 				current_cmd->args = calloc(2, sizeof(char*));
// 				if (current_cmd->args == NULL) {
// 					perror("Failed to allocate memory for args");
// 					free_cmd_list(*cmd_list);
// 					return;
// 				}
// 				// Set the first argument as the command itself
// 				current_cmd->args[0] = current_cmd->command;
// 			} else {
// 				// Find the current length of args
// 				int args_len;
// 				for (args_len = 0; current_cmd->args[args_len] != NULL; ++args_len);

// 				// Reallocate args to accommodate the new argument and the NULL terminator
// 				char **new_args = realloc(current_cmd->args, sizeof(char*) * (args_len + 2));
// 				if (new_args == NULL) {
// 					perror("Failed to reallocate memory for args");
// 					free_cmd_list(*cmd_list);
// 					return;
// 				}
// 				current_cmd->args = new_args;
// 				current_cmd->args[args_len] = strdup(parser->current_token->value);
// 				if (current_cmd->args[args_len] == NULL) {
// 					perror("Failed to duplicate argument string");
// 					free_cmd_list(*cmd_list);
// 					return;
// 				}
// 				// NULL terminate the args array
// 				current_cmd->args[args_len + 1] = NULL;
// 			}
//         }

//         parser_advance(parser);
//     }
// 	printf("cmd_list: %p\n", *cmd_list);
// 	printf("cmd_list->cmd->command: %s\n", (*cmd_list)->cmd->command);
// 	printf("cmd_list->cmd->args: %s\n", (*cmd_list)->cmd->args[0]);
// 	printf("cmd_list->cmd->args: %s\n", (*cmd_list)->cmd->args[1]);
// 	ft_execute(*cmd_list, env);
// }

// void parse_command(t_parser *parser, t_cmd_node **cmd_list, char **env) {
//     int cmd_flag;
//     t_cmd_node *current_node = NULL;
//     t_cmd *current_cmd = NULL;

//     cmd_flag = 1;
// 	env = NULL;
//     while (parser->current_token != NULL) {
//         // When cmd_flag is set, we need to allocate a new command node
// 				printf("ok\n");
//         if (cmd_flag == 1) {
//             if (*cmd_list == NULL) {
//                 *cmd_list = malloc(sizeof(t_cmd_node));
//                 current_node = *cmd_list;
//             } else {
//                 // Find the last node in the list
//                 current_node = *cmd_list;
//                 while (current_node->next != NULL) {
//                     current_node = current_node->next;
//                 }
//                 current_node->next = malloc(sizeof(t_cmd_node));
//                 current_node = current_node->next;
//             }
//             // Initialize the new command node
//             current_node->cmd = malloc(sizeof(t_cmd));
//             current_node->next = NULL;
//             current_cmd = current_node->cmd;
//             current_cmd->args = NULL; // Initialize args to NULL
//             current_cmd->command = NULL; // Initialize command to NULL
//             cmd_flag = 0; // Reset cmd_flag after handling a command.
//         }

//         // Handle CHAR_PIPE token
//         if (parser->current_token->type == CHAR_PIPE) {
//             cmd_flag = 1; // Set flag to handle a new command after the pipe.
//         } else if (parser->current_token->type == WORD) {
//             // If it's the first word, it's the command
//             if (current_cmd->command == NULL) {
//                 current_cmd->command = strdup(parser->current_token->value);
//             } else {
//                 // Otherwise, it's an argument
//                 // Count current arguments
//                 int arg_count = 0;
//                 while (current_cmd->args && current_cmd->args[arg_count]) {
//                     arg_count++;
//                 }
//                 // Allocate space for the new argument list
//                 char **new_args = malloc(sizeof(char *) * (arg_count + 2)); // +1 for new arg, +1 for NULL terminator
//                 for (int i = 0; i < arg_count; i++) {
//                     new_args[i] = current_cmd->args[i]; // Copy existing arguments
//                 }
//                 new_args[arg_count] = strdup(parser->current_token->value); // Add new argument
//                 new_args[arg_count + 1] = NULL; // NULL terminator
//                 free(current_cmd->args); // Free old argument list
//                 current_cmd->args = new_args; // Update command's arguments
//             }
//         }

//         parser_advance(parser); // Move to the next token
// 		// printf("current_cmd->command: %s\n", current_cmd->command);
//     }
// }


// void	parse(t_parser *parser, t_cmd *cmd, char **env)
// {
// 	parse_command(parser, cmd, env);
// }
