/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:58:50 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/05 12:39:48 by amtan            ###   ########.fr       */
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

static int	cmd_step(t_ast *cmd, t_token **head, t_token *end, int *i)
{
	t_token	*file;

	if ((*head)->type == SPCE)
		return ((*head = (*head)->next), 0);
	if ((*head)->type == TEXT)
	{
		cmd->args[*i] = ft_strdup((*head)->text);
		if (!cmd->args[*i])
			return (1);
		return ((*i)++, (*head = (*head)->next), 0);
	}
	if ((*head)->type == REDI_IN || (*head)->type == REDI_OT
		|| (*head)->type == APPEND || (*head)->type == HEREDOC)
	{
		file = (*head)->next;
		while (file && file != end && file->type == SPCE)
			file = file->next;
		if (!file || file == end || file->type != TEXT)
			return (1);
		if (!crea_rdir_push_back(&cmd->rdir, (*head)->type, file->text))
			return (1);
		return ((*head = file->next), 0);
	}
	return (1);
}

t_ast	*cre_ast_cmd_run(t_ast *cmd, t_token *head, t_token *tail)
{
	int		i;
	t_token	*end;

	i = 0;
	end = tail->next;
	while (head && head != end)
	{
		if (cmd_step(cmd, &head, end, &i))
			return (free_ast_one(&cmd), NULL);
	}
	return (cmd);
}

t_ast	*cre_ast_cmd(t_token *head, t_token *tail)
{
	t_ast	*cmd;
	int		i;

	cmd = cre_ast_node(AST_CMD);
	if (!cmd)
		return (NULL);
	i = count_ast_arg(head, tail);
	cmd->args = ft_calloc(i + 1, sizeof(char *));
	if (!cmd->args)
		return (free_ast_one(&cmd), NULL);
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
		return (cre_ast_logic(curr, head, tail));
	if (head->type != PAREN_L || tail->type != PAREN_R
		|| !matched_brkt(head, tail))
		return (cre_ast_cmd(head, tail));
	curr = head->next;
	while (curr && curr != tail && curr->type == SPCE)
		curr = curr->next;
	if (!curr || curr == tail)
		return (NULL);
	new = cre_ast_node((t_type)AST_BRKT);
	if (!new)
		return (NULL);
	new->left = build_ast_rec(head->next, tail->prev);
	if (!new->left)
		return (free_ast(new), NULL);
	return (new);
}
