/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:33:08 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/14 13:27:20 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

void	handle_pwd(void);
void	handle_cd(char **input);
void	handle_echo(char **input);
void	handle_env(char **env);
void	builtin_exec(char **input, char **env);

#endif
