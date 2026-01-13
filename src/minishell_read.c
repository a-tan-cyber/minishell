/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:13:43 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/13 11:32:19 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check for: {'', "", ()}
t_bool	line_is_complete(char *line)
{
	
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
		if (!line)
			return (ft_sfree(&rslt), NULL);
		line = ft_memappend_back(line, ft_strlen(line), "\n", 1);
		if (!line)
			return (ft_sfree(&rslt), NULL);
		temp = ft_memappend_back(rslt, ft_strlen(rslt), line, ft_strlen(line));
		ft_sfree(&line);
		rslt = temp;
		if (!temp)
			return (NULL);
		if (line_is_complete(rslt) == TRUE)
			return (rslt);
	}
	return (NULL);
}
