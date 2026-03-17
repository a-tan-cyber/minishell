/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hd_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:47:11 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:44:07 by amtan            ###   ########.fr       */
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
	if (ft_write_all(fd, out, ft_strlen(out))
		|| (i && i->interactive && ft_write_all(fd, "\n", 1)))
	{
		if (expand)
			free(out);
		return (0);
	}
	if (expand)
		free(out);
	return (1);
}

int	ms_hd_fail_one(t_info *i, t_redir *r, char **delim, int fd)
{
	if (fd >= 0 && close(fd) < 0 && g_sig != SIGINT)
		i->err = 1;
	if (r->file)
		unlink(r->file);
	ft_sfree((void **)&r->file);
	r->file = *delim;
	if (g_sig == SIGINT)
		return (g_sig = 0, i->err = 130, 1);
	i->err = 1;
	return (1);
}
