/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:14:25 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/20 12:45:03 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include "libft/libft.h"
# include "lexer.h"
// # include "parser.h"
# include "builtin.h"
# include <signal.h>

# define MAXARGS 10

enum e_cmd_types{
	PIPE = 1,
	LIST = 2,
	BACK = 3,
	EXEC = 4,
	REDIR = 5
};

typedef struct s_sig
{
	int		sigint;
	int		sigquit;
	int		exstatus;
	pid_t	pid;
}				t_sig;

typedef struct s_cmd
{
	char			*command;
	char			**args;
	char			*fd_in;
	char			*fd_out;
	pid_t			pid;
	int				status;
	struct s_cmd	*next;
}			t_cmd;

char	*expander_env(char *arg, char **env);
void	execute_command(char *command, char **args, int out_fd);
void	builtin_exec(char **input, char **env);
void	print_token(t_tok *token);
void	ft_execute(t_cmd *cmd, char **env);

char	*get_cmd_path(char *cmd, char **env);
void	free_str_array(char **array);
void	ft_error(const char *msg, int status);
void	ft_close_fd(int *pfd);

void ft_execute(t_cmd *cmd, char **env);
int	is_builtin(char *command);

#endif

// parsecmd
//	parsepipe
//	parseredir
//	parseback
//	parseexec
// 	parsebllock
// nullterminate
// gettoken

// main
// getcmd
// runcmd
// panic
// fork1
