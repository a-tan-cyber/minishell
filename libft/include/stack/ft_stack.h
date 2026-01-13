/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 11:47:01 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STACK_H
# define FT_STACK_H

# define STACKSIZE 1024

typedef struct s_stack
{
	void	*stack[STACKSIZE];
	int		i;
}			t_stack;

// ft_stack_base.c
int			initialise_stack(t_stack *new);
int			push_stack(t_stack *new, void *stuff);
void		*pop_stack(t_stack *new);
void		*peek(t_stack *new);

// ft_stack_chk.c
int			is_full(t_stack *new);
int			is_empty(t_stack *new);

#endif