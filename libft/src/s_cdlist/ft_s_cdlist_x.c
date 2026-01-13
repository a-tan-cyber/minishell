/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_s_dlist_x.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 22:37:59 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/12 22:37:59 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

t_cdlist	*ft_cdlist_swap_pos(t_cdlist *a, t_cdlist *b)
{
	if (!a || !b)
		return (NULL);
	a->next = b->next;
	b->next->prev = a;
	b->next = a;
	b->prev = a->prev;
	a->prev->next = b;
	a->prev = b;
	return (b);
}

void	ft_cdlist_insf(t_cdlist **head, t_cdlist *new)
{
	t_cdlist	*top;

	if (!head || !*head)
	{
		*head = new;
		return ;
	}
	if (!new)
		return ;
	top = *head;
	top->prev->next = new;
	new->next = top;
	new->prev = top->prev;
	top->prev = new;
	*head = new;
}
