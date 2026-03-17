/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:11:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/17 22:46:07 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	expand_tokens(t_token **lexed)
{
	t_token	*cur;
	t_bool	is_head;

	cur = *lexed;
	while (cur)
	{
		is_head = (cur == *lexed);
		if (cur->type == TEXT && cur->hd_delim == FALSE
			&& ft_strchr(cur->text, 127)
			&& expand_star(cur->text, &cur))
			return (free_token_lst(lexed), 2);
		if (is_head)
			*lexed = cur;
		cur = cur->next;
	}
	return (0);
}

static int	merge_adjacent_text_tokens(t_token **lexed)
{
	t_token	*cur;
	char	*new_raw;
	char	*new_text;

	cur = *lexed;
	while (cur)
	{
		if (cur->type == TEXT && cur->next && cur->next->type == TEXT)
		{
			new_text = ft_str_append(cur->text, cur->next->text);
			if (!new_text)
				return (free_token_lst(lexed), 1);
			cur->text = new_text;
			new_raw = ft_str_append(cur->raw, cur->next->raw);
			if (!new_raw)
				return (free_token_lst(lexed), 1);
			cur->raw = new_raw;
			cur->quoted = (cur->quoted || cur->next->quoted);
			cur->hd_delim = (cur->hd_delim || cur->next->hd_delim);
			free_token_one(&cur->next);
		}
		else
			cur = cur->next;
	}
	return (0);
}

int	merge_and_expand_tokens(t_token **lexed)
{
	if (merge_adjacent_text_tokens(lexed))
		return (1);
	if (split_unquoted_tokens(lexed))
		return (free_token_lst(lexed), 1);
	return (expand_tokens(lexed));
}
