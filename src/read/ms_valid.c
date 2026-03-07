/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_valid.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 07:40:06 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 12:28:38 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_bool	line_has_unpaired_char(char *line, char c)
{
	size_t	i;
	char	q;

	if (!line)
		return (FALSE);
	i = 0;
	q = 0;
	while (line[i])
	{
		q = ms_quote_next(q, line[i]);
		if (q == 0 && line[i] == c)
		{
			if (line[i + 1] != c)
				return (TRUE);
			i += 2;
			continue ;
		}
		i++;
	}
	return (FALSE);
}

static t_bool	line_has_bad_prefix(t_info *i, char *line)
{
	if (!ft_strncmp(line, "||", 2))
		return (ft_putendl_fd(
				"moonshell: syntax error near unexpected token `||'",
				STDERR_FILENO), i->err = 2, TRUE);
	if (!ft_strncmp(line, "&&", 2))
		return (ft_putendl_fd(
				"moonshell: syntax error near unexpected token `&&'",
				STDERR_FILENO), i->err = 2, TRUE);
	if (line[0] == '|')
		return (ft_putendl_fd(
				"moonshell: syntax error near unexpected token `|'",
				STDERR_FILENO), i->err = 2, TRUE);
	if (line[0] == ')')
		return (ft_putendl_fd(
				"moonshell: syntax error near unexpected token `)'",
				STDERR_FILENO), i->err = 2, TRUE);
	return (FALSE);
}

t_bool	line_valid(t_info *i, char **line)
{
	char	*temp;
	int		par;

	if (!i || !line || !*line)
		return (FALSE);
	temp = ft_strtrim_ws(*line);
	if (!temp)
		return (FALSE);
	ft_sfree((void **)line);
	*line = temp;
	if (ft_strcmp(*line, "") == 0)
		return (FALSE);
	if (line_has_bad_prefix(i, *line))
		return (FALSE);
	par = open_has_close(*line, '(', ')');
	if (par == -1)
		return (ft_putendl_fd(
				"moonshell: syntax error near unexpected token `)'",
				STDERR_FILENO), i->err = 2, FALSE);
	if (line_is_complete(*line) == FALSE)
		return (FALSE);
	if (line_has_unpaired_char(*line, '&') == TRUE)
		return (FALSE);
	return (TRUE);
}
