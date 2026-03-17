/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokenise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 21:43:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/17 23:19:51 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	tokenise_space(const char *line, t_token **lexed)
{
	size_t	i;

	i = 0;
	while (line[i] && ft_is_white_space(line[i]))
		i++;
	if (token_push_back(lexed, SPCE, NULL) == NULL)
		return (0);
	return (i);
}

size_t	tokenise_s_quote(const char *line, t_token **lexed, t_bool hd_delim)
{
	size_t	r;
	char	*raw;
	char	*text;
	t_token	*tok;

	r = ms_find_quote_end(line, '\'');
	if (!r)
		return (0);
	text = ft_strndup(line + 1, r - 1);
	if (!text)
		return (0);
	raw = ms_dup_raw(line, r + 1);
	if (!raw)
		return (ft_sfree((void **)&text), 0);
	tok = ms_push_text_token(lexed, text, raw);
	if (!tok)
		return (0);
	tok->quoted = TRUE;
	tok->hd_delim = hd_delim;
	return (r + 1);
}

size_t	tokenise_d_quote(const char *line, t_token **lexed, t_info *info,
		t_bool hd_delim)
{
	size_t	r;
	char	*raw;
	char	*text;
	t_token	*tok;

	r = ms_find_quote_end(line, '\"');
	if (!r)
		return (0);
	text = ft_strndup(line + 1, r - 1);
	if (!text)
		return (0);
	raw = ms_dup_raw(line, r + 1);
	if (!raw)
		return (ft_sfree((void **)&text), 0);
	if (!hd_delim)
		expand_dollar(&text, info);
	if (!text)
		return (ft_sfree((void **)&raw), 0);
	tok = ms_push_text_token(lexed, text, raw);
	if (!tok)
		return (0);
	tok->quoted = TRUE;
	tok->hd_delim = hd_delim;
	return (r + 1);
}

size_t	tokenise_oper(const char *line, t_token **lexed)
{
	size_t	res;

	res = 1;
	if (line[0] == '|' && line[1] == '|')
		res = tokenise_oper_or(lexed);
	else if (line[0] == '|')
		res = tokenise_oper_pipe(lexed);
	else if (line[0] == '&' && line[1] == '&')
		res = tokenise_oper_and(lexed);
	else if (line[0] == '<' && line[1] == '<')
		res = tokenise_oper_heredoc(lexed);
	else if (line[0] == '<')
		res = tokenise_oper_rediin(lexed);
	else if (line[0] == '>' && line[1] == '>')
		res = tokenise_oper_append(lexed);
	else if (line[0] == '>')
		res = tokenise_oper_rediot(lexed);
	return (res);
}

size_t	tokenise_brkt(const char *line, t_token **lexed)
{
	if (line[0] == '(')
	{
		if (token_push_back(lexed, PAREN_L, NULL) == NULL)
			return (0);
		return (1);
	}
	else if (line[0] == ')')
	{
		if (token_push_back(lexed, PAREN_R, NULL) == NULL)
			return (0);
		return (1);
	}
	return (0);
}
