/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_rdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:09:11 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/17 22:47:53 by amtan            ###   ########.fr       */
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
	new->ambig = FALSE;
	new->file = ft_strdup(file->text);
	if (!new->file)
		return (ft_sfree((void **)&new), NULL);
	if (file->raw)
		new->word = ft_strdup(file->raw);
	else
		new->word = ft_strdup(file->text);
	if (!new->word)
		return (ft_sfree((void **)&new->file), ft_sfree((void **)&new), NULL);
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

t_bool	ms_redir_is_ambig(t_token *oper, t_token *file, t_token *end)
{
	if (!oper || !file || oper->type == HEREDOC)
		return (FALSE);
	if (file->next && file->next != end && file->next->type == TEXT)
		return (TRUE);
	return (FALSE);
}

t_token	*ms_redir_skip_word(t_token *file, t_token *end)
{
	while (file && file != end && file->type == TEXT)
		file = file->next;
	return (file);
}
