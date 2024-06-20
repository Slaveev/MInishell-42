// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   signals.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/06/11 15:23:19 by jsamardz          #+#    #+#             */
// /*   Updated: 2024/06/17 14:55:34 by dslaveev         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// // signal hander for CTRL + C
// void	signal_handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		write(STDOUT_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

// void	sig_int(int sig)
// {
// 	(void)sig;
// 	if (g_sig.pid == 0)
// 	{
// 		write(2, "sig error\n", 10);
// 		g_sig.exstatus = 1;
// 	}
// 	else
// 	{
// 		write(2, "\n", 1);
// 		g_sig.exstatus = 130;
// 	}
// 	g_sig.sigint = 1;
// }

// void	sig_quit(int sig)
// {
// 	char	*nbr;

// 	if (g_sig.pid != 0)
// 	{
// 		nbr = ft_itoa(sig);
// 		write(2, "Quit: ", 6);
// 		write(2, nbr, ft_strlen(nbr));
// 		write(2, "\n", 1);
// 		g_sig.exstatus = 131;
// 		g_sig.sigquit = 1;
// 	}
// }

// void	signal_setup()
// {
// 	struct	sigaction sa;

// 	memset(&sa, 0, sizeof(sa));
// 	sa.sa_handler = signal_handler;
// 	sigaction(SIGINT, &sa, NULL);
// }

// void	inig_sig()
// {
// 	g_sig.pid = 0;
// 	g_sig.exstatus = 0;
// 	g_sig.sigquit = 0;
// 	g_sig.sigint = 0;
// }
