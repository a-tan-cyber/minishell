/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 13:28:07 by amtan             #+#    #+#             */
/*   Updated: 2026/03/10 19:59:43 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	hist_remove_all(t_hist **head, const char *line)
{
	t_hist	*cur;
	t_hist	*prev;
	t_hist	*next;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (ft_strcmp(cur->line, line) == 0)
		{
			if (prev)
				prev->next = next;
			else
				*head = next;
			ft_sfree((void **)&cur->line);
			ft_sfree((void **)&cur);
		}
		else
			prev = cur;
		cur = next;
	}
}

static int	hist_append(t_hist **head, const char *line)
{
	t_hist	*new;
	t_hist	*cur;

	new = ft_calloc(1, sizeof(t_hist));
	if (!new)
		return (1);
	new->line = ft_strdup(line);
	if (!new->line)
		return (ft_sfree((void **)&new), 1);
	if (!*head)
		return (*head = new, 0);
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (0);
}

void	ms_history_sync_readline(t_hist *head)
{
	rl_clear_history();
	while (head)
	{
		add_history(head->line);
		head = head->next;
	}
}

void	ms_history_add(t_info *i, const char *line)
{
	t_hist	*cur;

	if (!i || !i->interactive || !line || line[0] == '\0')
		return ;
	cur = i->hist;
	if (cur)
	{
		while (cur->next)
			cur = cur->next;
		if (ft_strcmp(cur->line, line) == 0)
			return ;
	}
	hist_remove_all(&i->hist, line);
	if (hist_append(&i->hist, line) == 0)
		ms_history_sync_readline(i->hist);
}

void	ms_history_clear(t_info *i)
{
	t_hist	*cur;
	t_hist	*next;

	if (!i)
		return ;
	cur = i->hist;
	while (cur)
	{
		next = cur->next;
		ft_sfree((void **)&cur->line);
		ft_sfree((void **)&cur);
		cur = next;
	}
	i->hist = NULL;
	rl_clear_history();
}
