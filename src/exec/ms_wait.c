/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:09:23 by amtan             #+#    #+#             */
/*   Updated: 2026/03/07 10:32:46 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_sigaction_set(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	return (sigaction(sig, &sa, NULL));
}

int	set_wait_signals(void)
{
	g_sig = 0;
	if (ms_sigaction_set(SIGINT, SIG_IGN) == -1)
		return (perror("sigaction(SIGINT)"), 1);
	if (ms_sigaction_set(SIGQUIT, SIG_IGN) == -1)
		return (perror("sigaction(SIGQUIT)"), 1);
	return (0);
}

void	set_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	ms_wait_pid(pid_t pid, int *status, int options)
{
	while (waitpid(pid, status, options) < 0)
	{
		if (errno != EINTR)
			return (1);
	}
	return (0);
}

int	ms_wait_status(int status, t_bool interactive, t_bool print_msg)
{
	if (print_msg && interactive && WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT && WCOREDUMP(status))
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit\n", 5);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
