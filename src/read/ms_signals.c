/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 05:56:54 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 11:00:19 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(STDOUT_FILENO, "^C", 2);
}

static int	ms_sigaction(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	return (sigaction(sig, &sa, NULL));
}

int	set_signals(void)
{
	if (ms_sigaction(SIGINT, handle_sigint) == -1)
	{
		perror("sigaction(SIGINT)");
		return (1);
	}
	if (ms_sigaction(SIGQUIT, SIG_IGN) == -1)
	{
		perror("sigaction(SIGQUIT)");
		return (1);
	}
	return (0);
}

char	*read_multiline_sigint(char *rslt)
{
	ft_sfree((void **)&rslt);
	return (ft_strdup(""));
}

int	rl_check_sigint(void)
{
	if (g_sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
	return (0);
}
