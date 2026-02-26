/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:13:43 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 19:57:34 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_bool	open_has_close(char *line, char open, char close)
{
	int	i;
	int	l;

	if (!line)
		return (FALSE);
	l = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == open)
		{
			l++;
		}
		if (line[i] == close)
		{
			if (l == 0)
				return (FALSE);
			l--;
		}
		i++;
	}
	return (l == 0);
}

// check for: {'', "", ()}
t_bool	line_is_complete(char *line)
{
	char	*temp;
	size_t	len;

	if (num_of_x_ignore_y_substr(line, '\'', '\"') % 2 != 0)
		return (FALSE);
	if (num_of_x_ignore_y_substr(line, '\"', '\'') % 2 != 0)
		return (FALSE);
	if (open_has_close(line, '(', ')') == FALSE)
		return (FALSE);
	temp = ft_strtrim_ws(line);
	if (!temp)
		return (FALSE);
	len = ft_strlen(temp);
	if (len >= 2 && (!ft_strcmp(temp + len - 2, "&&")
			|| !ft_strcmp(temp + len - 2, "||")))
		return (ft_sfree((void **)&temp), FALSE);
	ft_sfree((void **)&temp);
	return (TRUE);
}
