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
		return (ft_sfree(&temp), FALSE);
	ft_sfree(&temp);
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
		if (g_sig == SIGINT)
			return (read_multiline_sigint(rslt));
		line = ft_memappend_back(line, ft_strlen(line), "\n", 1);
		if (!line)
			return (ft_sfree(&rslt), NULL);
		temp = ft_memappend_back(rslt, ft_strlen(rslt), line, ft_strlen(line));
		ft_sfree(&line);
		rslt = temp;
		if (!temp)
			return (NULL);
		if (line_is_complete(rslt))
			return (rslt);
		msg = "> ";
	}
}
	// if (line_is_valid(rslt) == FALSE)
	// 	return ()
	// if (line_is_complete(rslt) == TRUE)
	// 	return (rslt);
	