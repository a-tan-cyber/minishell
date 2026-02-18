/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:26:20 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/12 15:26:20 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*cre_ast_node(t_type type)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	new->type = type;
	new->args = NULL;
	new->rdir = NULL;
	new->left = NULL;
	new->riht = NULL;
	return (new);
}

t_ast	*cre_ast_new(t_token *curr)
{
	t_ast	*new;

	new = NULL;
	if (curr->type == AND)
	{
		new = cre_ast_node((t_type)AND);
	}
	else if (curr->type == OR)
	{
		new = cre_ast_node((t_type)OR);
	}
	else if (curr->type == PIPE)
	{
		new = cre_ast_node((t_type)PIPE);
	}
	if (!new)
		return (NULL);
	return (new);
}

t_redir	*crea_rdir_push_back(t_redir **curr, t_oper type, char *text)
{
	t_redir	*last;

	if (!curr)
		return (NULL);
	if (!*curr)
	{
		*curr = crea_rdir_node(type, text);
		if (!*curr)
			return (NULL);
		return (*curr);
	}
	last = goto_rdir_last(*curr);
	last->next = crea_rdir_node(type, text);
	if (!last->next)
		return (NULL);
	return (*curr);
}
