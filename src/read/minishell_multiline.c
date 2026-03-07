/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_multiline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 19:56:29 by amtan             #+#    #+#             */
/*   Updated: 2026/03/07 12:39:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

static char	ml_unclosed_quote(const char *s)
{
	char	q;

	q = 0;
	while (*s)
	{
		q = ms_quote_next(q, *s);
		s++;
	}
	return (q);
}

static char	*ml_handle_eof(t_info *i, const char *first,
				const char *prompt, char *rslt)
{
	char	q;

	if (prompt == first)
		return (ft_sfree((void **)&rslt), NULL);
	q = ml_unclosed_quote(rslt);
	if (q != 0)
	{
		ft_putstr_fd("moonshell: unexpected EOF while looking for matching `",
			STDERR_FILENO);
		ft_putchar_fd(q, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
	else
		ft_putendl_fd("moonshell: syntax error: unexpected end of file",
			STDERR_FILENO);
	i->err = 2;
	ft_sfree((void **)&rslt);
	return (NULL);
}

static char	*ml_append_line(t_info *i, char *rslt, char *line)
{
	char	*tmp;
	size_t	len;

	len = ft_strlen(line);
	if (i->interactive || len == 0 || line[len - 1] != '\n')
	{
		line = ft_memappend_back(line, len, "\n", 1);
		if (!line)
			return (ft_sfree((void **)&rslt), NULL);
	}
	tmp = ft_memappend_back(rslt, ft_strlen(rslt), line, ft_strlen(line));
	ft_sfree((void **)&line);
	return (tmp);
}

static char	*ml_read_line(t_info *i, const char *prompt)
{
	char	*line;

	if (i->interactive)
	{
		rl_done = 0;
		line = readline(prompt);
		return (line);
	}
	line = get_next_line(STDIN_FILENO);
	return (line);
}

char	*read_multiline(t_info *i, const char *msg)
{
	char		*rslt;
	char		*line;
	const char	*prompt;

	prompt = msg;
	rslt = ft_strdup("");
	if (!rslt)
		return (NULL);
	while (TRUE)
	{
		line = ml_read_line(i, prompt);
		if (i->interactive && g_sig == SIGINT)
			return (free(line), read_multiline_sigint(rslt));
		if (!line)
			return (ml_handle_eof(i, msg, prompt, rslt));
		rslt = ml_append_line(i, rslt, line);
		if (!rslt)
			return (NULL);
		if (line_is_complete(rslt))
			return (rslt);
		if (!i->interactive && !*rslt)
			return (ft_sfree((void **)&rslt), ft_strdup(""));
		prompt = "> ";
	}
}
