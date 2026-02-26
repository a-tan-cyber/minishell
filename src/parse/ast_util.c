/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:10:02 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 21:35:25 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_ast_arg(t_token *head, t_token *tail)
{
	int		i;
	t_token	*end;

	if (!head || !tail)
		return (0);
	end = tail->next;
	i = 0;
	while (head && head != end)
	{
		if (head->type == SPCE)
		{
			head = head->next;
			continue ;
		}
		if (head->type == REDI_IN || head->type == REDI_OT
			|| head->type == APPEND || head->type == HEREDOC)
		{
			head = head->next;
			while (head && head != end && head->type == SPCE)
				head = head->next;
			if (head && head != end && head->type == TEXT)
				head = head->next;
			continue ;
		}
		if (head->type == TEXT)
			i++;
		head = head->next;
	}
	return (i);
}
