/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:58:50 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 17:58:50 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_token_logic_pipe(t_token *head, t_token *tail)
{
	t_token	*stor;

	if (!head || !tail)
		return (NULL);
	stor = tail;
	while (tail && tail != head)
	{
		if (tail->type == PAREN_R)
			tail = skip_token_brkt_rev(tail);
		else if (tail->type == AND || tail->type == OR)
			return (tail);
		else
			tail = tail->prev;
	}
	tail = stor;
	while (tail && tail != head)
	{
		if (tail->type == PAREN_R)
			tail = skip_token_brkt_rev(tail);
		else if (tail->type == PIPE)
			return (tail);
		else
			tail = tail->prev;
	}
	return (NULL);
}

t_ast	*cre_ast_cmd_run(t_ast *cmd, t_token *head, t_token *tail)
{
	int	i;

	i = 0;
	while (head)
	{
		if (head->type == TEXT)
		{
			cmd->args[i] = ft_strdup(head->text);
			if (!cmd->args[i])
				return (free_ast_one(&cmd), NULL);
			i++;
		}
		else
		{
			if (head->next &&
				!crea_rdir_push_back(&cmd->rdir,head->type, head->next->text))
				return (free_ast_one(&cmd), NULL);
			head = head->next;
		}
		if (head == tail)
			break ;
		head = head->next;
	}
	return (cmd);
}

t_ast	*cre_ast_cmd(t_token *head, t_token *tail)
{
	t_ast	*cmd;
	int		i;

	cmd = cre_ast_node(CMD);
	if (!cmd)
		return (NULL);
	i = count_ast_arg(head, tail);
	cmd->args = ft_calloc(i + 1, sizeof(char *));
	cmd = cre_ast_cmd_run(cmd, head, tail);
	if (!cmd)
		return (NULL);
	return (cmd);
}

t_ast	*build_ast_rec(t_token *head, t_token *tail)
{
	t_ast	*new;
	t_token	*curr;

	if (!head || !tail)
		return (NULL);
	curr = find_token_logic_pipe(head, tail);
	if (curr)
	{
		new = cre_ast_new(curr);
		if (!new)
			return (NULL);
		new->left = build_ast_rec(head, curr->prev);
		new->riht = build_ast_rec(curr->next, tail);
		return (new);
	}
	if (head->type == PAREN_L && tail->type == PAREN_R && matched_brkt(head, tail))
	{
		new = cre_ast_node((t_type)BRKT);
		if (!new)
			return (NULL);
		new->left = build_ast_rec(head->next, tail->prev);
		return (new);
	}
	return (cre_ast_cmd(head, tail));
}


