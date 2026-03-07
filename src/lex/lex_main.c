/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 18:40:38 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 17:27:02 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_bool	is_hd_delim_word(t_token *lexed)
{
	t_token	*last;

	last = token_last(lexed);
	if (!last)
		return (FALSE);
	if (last->type == TEXT && last->hd_delim)
		return (TRUE);
	while (last && last->type == SPCE)
		last = last->prev;
	if (last && last->type == HEREDOC)
		return (TRUE);
	return (FALSE);
}

static size_t	count_newlines(const char *line, size_t len)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	while (i < len)
	{
		if (line[i] == '\n')
			n++;
		i++;
	}
	return (n);
}

static size_t	tokenise_text(const char *line, t_token **lexed, t_info *info,
		t_bool hd_delim)
{
	size_t	r;
	char	*text;
	t_token	*tok;

	r = 0;
	while (line[r] && !is_operator(line[r]) && !ft_is_white_space(line[r])
		&& line[r] != '\'' && line[r] != '\"' && !is_bracket(line[r]))
		r++;
	text = ft_strndup(line, r);
	if (!text)
		return (0);
	if (!hd_delim)
	{
		if (expand_dollar(&text, info) == 0)
			return (ft_sfree((void **)&text), 0);
		ft_str_replace_chr(text, '*', 127);
	}
	tok = token_push_back(lexed, TEXT, text);
	if (!tok)
		return (ft_sfree((void **)&text), 0);
	tok->hd_delim = hd_delim;
	return (r);
}

static size_t	tokenise_piece(const char *line, t_token **lexed, t_info *info)
{
	t_bool	hd_delim;

	hd_delim = is_hd_delim_word(*lexed);
	if (ft_is_white_space(line[0]))
		return (tokenise_space(line, lexed));
	if (line[0] == '\'')
		return (tokenise_s_quote(line, lexed, hd_delim));
	if (line[0] == '\"')
		return (tokenise_d_quote(line, lexed, info, hd_delim));
	if (is_operator(line[0]))
		return (tokenise_oper(line, lexed));
	if (is_bracket(line[0]))
		return (tokenise_brkt(line, lexed));
	return (tokenise_text(line, lexed, info, hd_delim));
}

int	lex_line(const char *line, t_token **lexed, t_info *info)
{
	size_t	i;
	size_t	len;
	int		line_no;
	t_token	*prev;
	t_token	*last;

	i = 0;
	line_no = info->cmd_line_no;
	while (line[i])
	{
		prev = token_last(*lexed);
		len = tokenise_piece(line + i, lexed, info);
		if (len == 0)
			return (free_token_lst(lexed), -1);
		last = token_last(*lexed);
		if (last && last != prev)
			last->line_no = line_no;
		line_no += count_newlines(line + i, len);
		i += len;
	}
	return (merge_and_expand_tokens(lexed));
}
