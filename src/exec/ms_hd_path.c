/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hd_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:47:11 by amtan             #+#    #+#             */
/*   Updated: 2026/03/07 14:51:40 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ms_hd_path(int idx)
{
	char	*num;
	char	*path;

	num = ft_itoa(idx);
	if (!num)
		return (NULL);
	path = ft_strjoin2("/tmp/moonshell_hd_", num);
	ft_sfree((void **)&num);
	return (path);
}

char	*ms_hd_next_line(t_info *i)
{
	if (i && i->interactive)
	{
		rl_done = 0;
		return (readline("> "));
	}
	return (get_next_line(STDIN_FILENO));
}

t_bool	ms_hd_is_delim(char *line, const char *delim)
{
	size_t	len;

	if (!line || !delim)
		return (FALSE);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	if (ft_strlen(delim) != len)
		return (FALSE);
	return (!ft_strncmp(line, delim, len));
}

int	ms_hd_write_line(t_info *i, int fd, char *line, t_bool expand)
{
	char	*out;

	out = line;
	if (expand)
	{
		out = ft_strdup(line);
		if (!out || !expand_dollar(&out, i))
			return (0);
	}
	write(fd, out, ft_strlen(out));
	if (i && i->interactive)
		write(fd, "\n", 1);
	if (expand)
		free(out);
	return (1);
}

int	ms_hd_fail_one(t_info *i, char **path, int fd)
{
	close(fd);
	unlink(*path);
	ft_sfree((void **)path);
	if (g_sig == SIGINT)
		return (g_sig = 0, i->err = 130, 1);
	return (1);
}
