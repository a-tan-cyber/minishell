/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_split_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 21:33:52 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:20:54 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	append_field(t_token **lst, t_token *src, size_t i, size_t j)
{
	char	*copy;
	char	*text;
	t_token	*tok;

	text = ft_strndup(src->text + i, j - i);
	if (!text)
		return (1);
	copy = NULL;
	if (src->raw)
	{
		copy = ft_strdup(src->raw);
		if (!copy)
			return (ft_sfree((void **)&text), 1);
	}
	tok = token_push_back(lst, TEXT, text);
	if (!tok)
		return (ft_sfree((void **)&copy), ft_sfree((void **)&text), 1);
	tok->raw = copy;
	tok->line_no = src->line_no;
	return (0);
}

static void	link_split_list(t_token **lexed, t_token *cur, t_token *split)
{
	t_token	*last;

	last = token_last(split);
	if (cur->prev)
		cur->prev->next = split;
	else
		*lexed = split;
	split->prev = cur->prev;
	last->next = cur->next;
	if (cur->next)
		cur->next->prev = last;
}

static void	replace_split_token(t_token **lexed, t_token *cur,
		t_token *split, t_token **next)
{
	if (split)
		link_split_list(lexed, cur, split);
	else if (cur->prev)
		cur->prev->next = cur->next;
	else
		*lexed = cur->next;
	if (!split && cur->next)
		cur->next->prev = cur->prev;
	if (split)
		*next = token_last(split)->next;
	else
		*next = cur->next;
	ft_sfree((void **)&cur->text);
	ft_sfree((void **)&cur->raw);
	ft_sfree((void **)&cur);
}

static int	split_unquoted_token(t_token **lexed, t_token *cur, t_token **next)
{
	t_token	*split;
	size_t	i;
	size_t	j;

	if (cur->type != TEXT || cur->quoted || cur->hd_delim)
		return (*next = cur->next, 0);
	split = NULL;
	i = 0;
	while (cur->text[i])
	{
		while (cur->text[i] && ft_is_white_space(cur->text[i]))
			i++;
		j = i;
		while (cur->text[j] && !ft_is_white_space(cur->text[j]))
			j++;
		if (j > i && append_field(&split, cur, i, j))
			return (free_token_lst(&split), 1);
		i = j;
	}
	if (split && !split->next && !ft_strcmp(split->text, cur->text))
		return (*next = cur->next, free_token_lst(&split), 0);
	replace_split_token(lexed, cur, split, next);
	return (0);
}

int	split_unquoted_tokens(t_token **lexed)
{
	t_token	*cur;
	t_token	*next;

	cur = *lexed;
	while (cur)
	{
		if (split_unquoted_token(lexed, cur, &next))
			return (1);
		cur = next;
	}
	return (0);
}
