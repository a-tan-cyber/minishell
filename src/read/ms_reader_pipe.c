/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_reader_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 20:02:25 by amtan             #+#    #+#             */
/*   Updated: 2026/03/13 16:11:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_reader_write_all(int fd, const void *buf, size_t len)
{
	size_t			off;
	ssize_t			wr;
	const char		*ptr;

	off = 0;
	ptr = (const char *)buf;
	while (off < len)
	{
		wr = write(fd, ptr + off, len - off);
		if (wr < 0)
		{
			if (errno == EINTR)
				continue ;
			return (1);
		}
		off += (size_t)wr;
	}
	return (0);
}

static int	ms_reader_read_all(int fd, void *buf, size_t len)
{
	size_t	off;
	ssize_t	rd;

	off = 0;
	while (off < len)
	{
		rd = read(fd, (char *)buf + off, len - off);
		if (rd == 0)
			return (2);
		if (rd < 0)
		{
			if (errno == EINTR && g_sig != SIGINT)
				continue ;
			return (1);
		}
		off += (size_t)rd;
	}
	return (0);
}

static int	ms_reader_send_line(int fd, char *line)
{
	int	len;

	if (!line)
		len = -1;
	else
		len = (int)ft_strlen(line);
	if (ms_reader_write_all(fd, &len, sizeof(len)))
		return (1);
	if (len <= 0)
		return (0);
	return (ms_reader_write_all(fd, line, (size_t)len));
}

int	ms_reader_recv_line(int fd, char **line)
{
	int	len;
	int	ret;

	*line = NULL;
	ret = ms_reader_read_all(fd, &len, sizeof(len));
	if (ret == 2)
		return (1);
	if (ret != 0)
		return (2);
	if (len < 0)
		return (1);
	*line = ft_calloc((size_t)len + 1, 1);
	if (!*line)
		return (2);
	if (len == 0)
		return (0);
	ret = ms_reader_read_all(fd, *line, (size_t)len);
	if (ret != 0)
		return (ft_sfree((void **)line), 2);
	return (0);
}

void	ms_reader_child_run(t_info *i, const char *prompt, int fd)
{
	char	*line;

	set_reader_signals();
	line = readline(prompt);
	if (ms_reader_send_line(fd, line))
	{
		free(line);
		close(fd);
		ms_exit_child(i, 1);
	}
	free(line);
	close(fd);
	ms_exit_child(i, 0);
}
