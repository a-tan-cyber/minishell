/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_var_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:40:19 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 14:40:51 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_var	*ms_var_new(const char *name, const char *value, t_bool exported)
{
	t_var	*new;

	new = ft_calloc(1, sizeof(t_var));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
		return (ft_sfree((void **)&new), NULL);
	if (value)
	{
		new->value = ft_strdup(value);
		if (!new->value)
		{
			ft_sfree((void **)&new->name);
			ft_sfree((void **)&new);
			return (NULL);
		}
	}
	new->exported = exported;
	return (new);
}

void	ms_var_add_back(t_var **lst, t_var *node)
{
	t_var	*cur;

	if (!lst || !node)
		return ;
	if (!*lst)
	{
		*lst = node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

void	ms_var_clear(t_var **lst)
{
	t_var	*cur;
	t_var	*next;

	if (!lst || !*lst)
		return ;
	cur = *lst;
	while (cur)
	{
		next = cur->next;
		ft_sfree((void **)&cur->name);
		ft_sfree((void **)&cur->value);
		ft_sfree((void **)&cur);
		cur = next;
	}
	*lst = NULL;
}
