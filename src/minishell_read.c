/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:13:43 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/17 11:33:32 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (ft_count_cins(line, '\'') % 2 != 0)
		return (FALSE);
	if (ft_count_cins(line, '\"') % 2 != 0)
		return (FALSE);
	if (open_has_close(line, '(', ')') == FALSE)
		return (FALSE);
	return (TRUE);
}

// adds '\n' at the end of each line
// returns multiline char* separated by \n
char	*read_multiline(char *msg)
{
	char	*line;
	char	*temp;
	char	*rslt;

	rslt = ft_strdup("");
	if (!rslt)
		return (NULL);
	while (TRUE)
	{
		line = readline(msg);
		line = ft_memappend_back(line, ft_strlen(line), "\n", 1);
		if (!line)
			return (ft_sfree(&rslt), NULL);
		temp = ft_memappend_back(rslt, ft_strlen(rslt), line, ft_strlen(line));
		ft_sfree(&line);
		rslt = temp;
		if (!temp)
			return (NULL);
	}
	return (NULL);
}
	// if (line_is_valid(rslt) == FALSE)
	// 	return ()
	// if (line_is_complete(rslt) == TRUE)
	// 	return (rslt);
	