/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:11:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 18:11:24 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_tokens(t_token **lexed)
{
	t_token	*cur;
	t_bool	is_head;

	cur = *lexed;
	while (cur)
	{
		is_head = (cur == *lexed);
		if (cur->type == TEXT && ft_strchr(cur->text, 127) &&
			expand_star(cur->text, &cur))
			return (free_token_lst(lexed), 2);
		if (is_head)
			*lexed = cur;
		cur = cur->next;
	}
	return (0);
}

// *; success returns 0, failure returns -1;
int	merge_and_expand_tokens(t_token **lexed)
{
	t_token	*cur;
	t_token	*new;
	int		err;

	cur = *lexed;
	while (cur)
	{
		if (cur->type == TEXT && cur->next && cur->next->type == TEXT)
		{
			cur->text = ft_str_append(cur->text, cur->next->text);
			if (!cur->text)
				return (free_token_lst(lexed), 1);
			free_token_one(&cur->next);
		}
		else
			cur = cur->next;
	}
	err = expand_tokens(lexed);
	return (err);
}