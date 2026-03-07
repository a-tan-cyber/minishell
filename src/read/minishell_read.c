/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:13:43 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 12:15:38 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_has_close(char *line, char open, char close)
{
	int		i;
	int		l;
	char	q;

	if (!line)
		return (0);
	i = 0;
	l = 0;
	q = 0;
	while (line[i])
	{
		q = ms_quote_next(q, line[i]);
		if (q == 0 && line[i] == open)
			l++;
		else if (q == 0 && line[i] == close)
		{
			if (l == 0)
				return (-1);
			l--;
		}
		i++;
	}
	if (l == 0)
		return (1);
	return (0);
}

static t_bool	line_ends_in_open_op(char *s, size_t len)
{
	if (len >= 2 && (!ft_strcmp(s + len - 2, "&&")
			|| !ft_strcmp(s + len - 2, "||")
			|| !ft_strcmp(s + len - 2, "<<")
			|| !ft_strcmp(s + len - 2, ">>")))
		return (TRUE);
	if (len >= 1 && (s[len - 1] == '|'
			|| s[len - 1] == '<'
			|| s[len - 1] == '>'))
		return (TRUE);
	return (FALSE);
}

// check for: {'', "", ()}
t_bool	line_is_complete(char *line)
{
	char	q;
	char	*temp;
	size_t	len;
	size_t	i;

	if (!line)
		return (FALSE);
	q = 0;
	i = 0;
	while (line[i])
		q = ms_quote_next(q, line[i++]);
	if (q != 0 || open_has_close(line, '(', ')') == 0)
		return (FALSE);
	temp = ft_strtrim_ws(line);
	if (!temp)
		return (FALSE);
	len = ft_strlen(temp);
	if (line_ends_in_open_op(temp, len))
		return (ft_sfree((void **)&temp), FALSE);
	return (ft_sfree((void **)&temp), TRUE);
}
