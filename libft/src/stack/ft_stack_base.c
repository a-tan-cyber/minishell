/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 12:03:43 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	initialise_stack(t_stack *new)
{
	new->i = 0;
	return (0);
}

int	push_stack(t_stack *new, void *stuff)
{
	if (new->i >= STACKSIZE)
	{
		return (999);
	}
	new->stack[new->i] = stuff;
	new->i++;
	return (0);
}

void	*pop_stack(t_stack *new)
{
	if (new->i == 0)
		return (0);
	new->i--;
	return (new->stack[new->i]);
}

void	*peek(t_stack *new)
{
	if (new->i == 0)
		return (0);
	return (new->stack[new->i - 1]);
}
