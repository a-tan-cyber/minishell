/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_rdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:09:11 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 17:18:55 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir	*crea_rdir_node(t_token *oper, t_token *file)
{
	t_redir	*new;

	if (!oper || !file)
		return (NULL);
	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->type = oper->type;
	new->line_no = oper->line_no;
	new->is_hd_tmp = FALSE;
	new->hd_expand = (oper->type == HEREDOC && file->quoted == FALSE);
	new->file = ft_strdup(file->text);
	if (!new->file)
		return (ft_sfree((void **)&new), NULL);
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
