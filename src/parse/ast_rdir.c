/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_rdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:09:11 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 18:09:11 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir	*crea_rdir_node(t_oper type, char *text)
{
	t_redir	*new;

	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->type = type;
	new->file = ft_strdup(text);
	if (!new->file)
		return (ft_sfree(&new), NULL);
	return (new);
}

t_redir	*goto_rdir_last(t_redir *curr)
{
	t_redir	*temp;

	temp = curr;
	while (curr)
	{
		temp = curr;
		curr = curr->next;
	}
	return (temp);
}
