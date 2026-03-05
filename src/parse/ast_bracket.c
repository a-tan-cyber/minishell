/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_bracket.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:07:56 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/05 11:39:20 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	matched_brkt(t_token *head, t_token *tail)
{
	int	left;

	if (!head || !tail || head->type != PAREN_L)
		return (0);
	left = 0;
	while (head && head != tail->next)
	{
		if (head->type == PAREN_L)
			left++;
		else if (head->type == PAREN_R)
			left--;
		if (left == 0)
			return (head == tail);
		head = head->next;
	}
	return (0);
}

//skip parenthesis reverse
t_token	*skip_token_brkt_rev(t_token *tail)
{
	int	r;

	if (!tail || tail->type != PAREN_R)
		return (NULL);
	r = 0;
	while (tail)
	{
		if (tail->type == PAREN_R)
			r++;
		else if (tail->type == PAREN_L)
			r--;
		tail = tail->prev;
		if (r == 0)
			return (tail);
	}
	return (NULL);
}
