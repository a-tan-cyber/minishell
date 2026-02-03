/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_valid.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 07:40:06 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/21 07:40:06 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_bool	line_has_unpaired_char(char *line, char c)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
		{
			i++;
			while (line[i] && line[i++] != '\'');
		}
		else if (line[i] == '\"')
		{
			i++;
			while (line[i] && line[i++] != '\"');
		}
		else if (line[i] == c)
		{
			if (line[i + 1] != c)
				return (TRUE);
			i += 2;
		}
		else
			i++;
	}
	return (FALSE);
}

t_bool	line_valid(char **line)
{
	char	*temp;

	if (!line || !*line)
		return (FALSE);
	temp = ft_strtrim(*line, " \t\n\r\v\f");
	if (!temp)
		return (FALSE);
	ft_sfree((void **)line);
	*line = temp;
	if (ft_strcmp(*line, "") == 0)
		return (FALSE);
	if (line_is_complete(*line) == FALSE)
		return (FALSE);
	if (line_has_unpaired_char(*line, '&') == TRUE)
		return (FALSE);
	return (TRUE);
}
