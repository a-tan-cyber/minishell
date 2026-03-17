/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 23:15:09 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:15:17 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ms_find_text_end(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && !is_operator(line[i]) && !ft_is_white_space(line[i])
		&& line[i] != '\'' && line[i] != '\"' && !is_bracket(line[i]))
		i++;
	return (i);
}

size_t	ms_find_quote_end(const char *line, char quote)
{
	size_t	i;

	i = 1;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (0);
	return (i);
}

char	*ms_dup_raw(const char *line, size_t len)
{
	return (ft_strndup(line, len));
}

t_token	*ms_push_text_token(t_token **lexed, char *text, char *raw)
{
	t_token	*tok;

	tok = token_push_back(lexed, TEXT, text);
	if (!tok)
	{
		ft_sfree((void **)&raw);
		ft_sfree((void **)&text);
		return (NULL);
	}
	tok->raw = raw;
	return (tok);
}
