/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quote.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 10:40:44 by amtan             #+#    #+#             */
/*   Updated: 2026/03/12 23:26:40 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ms_input_pop(char **buf)
{
	char	*line;
	char	*nl;
	char	*rest;

	if (!buf || !*buf)
		return (NULL);
	nl = ft_strchr(*buf, '\n');
	if (!nl)
		return (line = *buf, *buf = NULL, line);
	line = ft_strndup(*buf, nl - *buf);
	if (!line)
		return (ft_sfree((void **)buf), NULL);
	rest = ft_strdup(nl + 1);
	ft_sfree((void **)buf);
	if (!rest && nl[1] != '\0')
		return (ft_sfree((void **)&line), NULL);
	if (rest && rest[0] == '\0')
		ft_sfree((void **)&rest);
	*buf = rest;
	return (line);
}

static char	*ms_input_split_store(t_info *i, char *line)
{
	char	*nl;
	char	*rest;
	char	*head;

	nl = ft_strchr(line, '\n');
	if (!nl)
		return (line);
	head = ft_strndup(line, nl - line);
	if (!head)
		return (ft_sfree((void **)&line), NULL);
	rest = ft_strdup(nl + 1);
	ft_sfree((void **)&line);
	if (!rest && nl[1] != '\0')
		return (ft_sfree((void **)&head), NULL);
	if (rest && rest[0] == '\0')
		ft_sfree((void **)&rest);
	i->input_buf = rest;
	return (head);
}

char	*ms_input_next_line(t_info *i, const char *prompt)
{
	char	*line;

	if (i && i->input_buf)
		return (ms_input_pop(&i->input_buf));
	if (i && i->from_arg)
		return (NULL);
	if (i && i->interactive)
	{
		line = ms_readline_forked(i, prompt);
		if (!line)
			return (NULL);
		return (ms_input_split_store(i, line));
	}
	return (get_next_line(STDIN_FILENO));
}

char	ms_quote_next(char q, char c)
{
	if (c == '\'' && q != '"')
	{
		if (q == '\'')
			return (0);
		return ('\'');
	}
	if (c == '"' && q != '\'')
	{
		if (q == '"')
			return (0);
		return ('"');
	}
	return (q);
}
