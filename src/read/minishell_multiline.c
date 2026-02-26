/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_multiline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 19:56:29 by amtan             #+#    #+#             */
/*   Updated: 2026/02/26 19:56:35 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ml_handle_eof(const char *first, const char *prompt, char *rslt)
{
	if (prompt == first)
		return (ft_sfree((void **)&rslt), NULL);
	write(1, "\n", 1);
	ft_sfree((void **)&rslt);
	return (ft_strdup(""));
}

static char	*ml_append_line(char *rslt, char *line)
{
	char	*tmp;

	line = ft_memappend_back(line, ft_strlen(line), "\n", 1);
	if (!line)
		return (ft_sfree((void **)&rslt), NULL);
	tmp = ft_memappend_back(rslt, ft_strlen(rslt), line, ft_strlen(line));
	ft_sfree((void **)&line);
	return (tmp);
}

char	*read_multiline(const char *msg)
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
		rl_done = 0;
		line = readline(prompt);
		if (g_sig == SIGINT)
			return (free(line), read_multiline_sigint(rslt));
		if (!line)
			return (ml_handle_eof(msg, prompt, rslt));
		rslt = ml_append_line(rslt, line);
		if (!rslt)
			return (NULL);
		if (line_is_complete(rslt))
			return (rslt);
		prompt = "> ";
	}
}