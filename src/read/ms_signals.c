/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 05:56:54 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 19:06:20 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "^C", 2);
}

void	set_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

char	*read_multiline_sigint(char *rslt)
{
	g_sig = 0;
	ft_sfree((void **)&rslt);
	return (ft_strdup(""));
}

int	rl_check_sigint(void)
{
	if (g_sig == SIGINT)
	{
		rl_replace_line("", 1);
		rl_done = 1;
	}
	return (0);
}