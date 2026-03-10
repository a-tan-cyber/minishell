/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_reader_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 20:01:59 by amtan             #+#    #+#             */
/*   Updated: 2026/03/10 20:02:02 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_reader_open(int pfd[2])
{
	if (pipe(pfd) == -1)
		return (perror("pipe"), 1);
	return (0);
}

static pid_t	ms_reader_spawn(t_info *i, const char *prompt, int pfd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid != 0)
		return (pid);
	close(pfd[0]);
	ms_reader_child_run(i, prompt, pfd[1]);
	exit(1);
}

static char	*ms_reader_finish(t_info *i, pid_t pid, int fd)
{
	char	*line;
	int		kind;
	int		status;

	kind = ms_reader_recv_line(fd, &line);
	close(fd);
	if (ms_wait_pid(pid, &status, 0))
		return (i->err = 1, ft_sfree((void **)&line), NULL);
	if (g_sig == SIGINT)
		return (ft_sfree((void **)&line), NULL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (ft_sfree((void **)&line), NULL);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		return (i->err = 1, ft_sfree((void **)&line), NULL);
	if (kind == 1)
		return (NULL);
	if (kind != 0)
		return (i->err = 1, ft_sfree((void **)&line), NULL);
	return (line);
}

char	*ms_readline_forked(t_info *i, const char *prompt)
{
	int		pfd[2];
	pid_t	pid;

	if (ms_reader_open(pfd))
		return (i->err = 1, NULL);
	pid = ms_reader_spawn(i, prompt, pfd);
	if (pid < 0)
	{
		perror("fork");
		close(pfd[0]);
		close(pfd[1]);
		return (i->err = 1, NULL);
	}
	close(pfd[1]);
	return (ms_reader_finish(i, pid, pfd[0]));
}
