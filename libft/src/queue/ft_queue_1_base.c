/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_queue_1_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 12:17:35 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/queue/ft_queue.h"

int	initialise_queue_1(t_cir_buf_1 *new)
{
	new->size = 0;
	new->front = 0;
	new->back = 0;
	return (0);
}

int	enqueue_1(t_cir_buf_1 *queue, void *new)
{
	if (queue->size == QUEUESIZE)
		return (FTQ_EFULL);
	queue->buf[queue->back] = new;
	queue->back = (queue->back + 1) % QUEUESIZE;
	queue->size++;
	return (0);
}

void	*dequeue_1(t_cir_buf_1 *queue)
{
	void	*temp;

	if (queue->size <= 0)
		return (0);
	temp = queue->buf[queue->front];
	queue->front = (queue->front + 1) % QUEUESIZE;
	queue->size--;
	return (temp);
}

void	*peek_front_1(t_cir_buf_1 *queue)
{
	if (is_empty_1(queue))
		return (0);
	return (queue->buf[queue->front]);
}

void	*peek_back_1(t_cir_buf_1 *queue)
{
	int	i;

	if (is_empty_1(queue))
		return (0);
	i = queue->back - 1 + QUEUESIZE;
	i = i % QUEUESIZE;
	return (queue->buf[i]);
}
