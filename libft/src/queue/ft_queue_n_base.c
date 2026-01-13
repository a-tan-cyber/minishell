/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_queue_n_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 12:17:44 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	initialise_queue_n(t_cir_buf_n *new)
{
	new->size = 0;
	new->front = 0;
	new->back = 0;
	return (0);
}

int	enqueue_n(t_cir_buf_n *queue, void *new, void *new2)
{
	if (queue->size == QUEUESIZE)
		return (FTQ_EFULL);
	queue->buf[queue->back] = new;
	queue->buf2[queue->back] = new2;
	queue->back = (queue->back + 1) % QUEUESIZE;
	queue->size++;
	return (0);
}

int	dequeue_n(t_cir_buf_n *queue)
{
	int	temp;

	if (queue->size <= 0)
		return (FTQ_EEMPTY);
	temp = queue->front;
	queue->front = (queue->front + 1) % QUEUESIZE;
	queue->size--;
	return (temp);
}

int	peek_front_n(t_cir_buf_n *queue)
{
	if (is_empty_n(queue))
		return (FTQ_EEMPTY);
	return (queue->front);
}

int	peek_back_n(t_cir_buf_n *queue)
{
	int	i;

	if (is_empty_n(queue))
		return (FTQ_EEMPTY);
	i = queue->back - 1 + QUEUESIZE;
	i = i % QUEUESIZE;
	return (i);
}
