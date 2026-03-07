/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hd_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:47:11 by amtan             #+#    #+#             */
/*   Updated: 2026/03/07 12:48:00 by amtan            ###   ########.fr       */
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

void	ms_hd_write_line(t_info *i, int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	if (i && i->interactive)
		write(fd, "\n", 1);
}
