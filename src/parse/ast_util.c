/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:10:02 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/01 17:23:24 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_bool	ast_is_redir(t_oper type)
{
	return (type == REDI_IN || type == REDI_OT
		|| type == APPEND || type == HEREDOC);
}

static t_token	*ast_skip_spaces(t_token *tok, t_token *end)
{
	while (tok && tok != end && tok->type == SPCE)
		tok = tok->next;
	return (tok);
}

static t_token	*ast_skip_redir(t_token *tok, t_token *end)
{
	tok = tok->next;
	tok = ast_skip_spaces(tok, end);
	if (tok && tok != end && tok->type == TEXT)
		tok = tok->next;
	return (tok);
}

int	count_ast_arg(t_token *head, t_token *tail)
{
	int		i;
	t_token	*end;

	if (!head || !tail)
		return (0);
	i = 0;
	end = tail->next;
	while (head && head != end)
	{
		head = ast_skip_spaces(head, end);
		if (!head || head == end)
			break ;
		if (ast_is_redir(head->type))
			head = ast_skip_redir(head, end);
		else
		{
			if (head->type == TEXT)
				i++;
			head = head->next;
		}
	}
	return (i);
}
