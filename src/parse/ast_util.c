/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:10:02 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 18:10:02 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_ast_arg(t_token *head, t_token *tail)
{
	int	i;

	if (!head || !tail)
		return (0);
	i = 0;
	while (TRUE)
	{
		if (head->type == TEXT)
			i++;
		if (!head || head == tail)
			break ;
		head = head->next;
	}
	return (i);
}
