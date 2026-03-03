/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:13:43 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/03 16:14:31 by amtan            ###   ########.fr       */
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
	if (len >= 2 && (!ft_strcmp(temp + len - 2, "&&")
			|| !ft_strcmp(temp + len - 2, "||")))
		return (ft_sfree((void **)&temp), FALSE);
	return (ft_sfree((void **)&temp), TRUE);
}
