/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_valid_lex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 23:00:22 by amtan             #+#    #+#             */
/*   Updated: 2026/03/08 23:00:37 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	lex_syntax_error(t_info *i, char *token)
{
	ft_putstr_fd("moonshell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	i->err = 2;
}

static char	*lex_token_str(t_oper type)
{
	if (type == PIPE)
		return ("|");
	if (type == REDI_IN)
		return ("<");
	if (type == REDI_OT)
		return (">");
	if (type == APPEND)
		return (">>");
	if (type == HEREDOC)
		return ("<<");
	if (type == AND)
		return ("&&");
	if (type == OR)
		return ("||");
	if (type == PAREN_L)
		return ("(");
	if (type == PAREN_R)
		return (")");
	return ("newline");
}

static t_bool	lex_bad_seq(t_token *prev, t_token *cur)
{
	if (!prev && (cur->type == PIPE || cur->type == AND
			|| cur->type == OR || cur->type == PAREN_R))
		return (TRUE);
	if (prev && (prev->type == PIPE || prev->type == AND
			|| prev->type == OR) && (cur->type == PIPE
			|| cur->type == AND || cur->type == OR
			|| cur->type == PAREN_R))
		return (TRUE);
	if (prev && (prev->type == REDI_IN || prev->type == REDI_OT
			|| prev->type == APPEND || prev->type == HEREDOC)
		&& cur->type != TEXT)
		return (TRUE);
	return (FALSE);
}

t_bool	lex_valid(t_info *i, t_token *lexed)
{
	t_token	*prev;
	char	*tok;

	prev = NULL;
	while (lexed)
	{
		if (lexed->type == SPCE)
		{
			lexed = lexed->next;
			continue ;
		}
		tok = lex_token_str(lexed->type);
		if (lex_bad_seq(prev, lexed))
			return (lex_syntax_error(i, tok), FALSE);
		prev = lexed;
		lexed = lexed->next;
	}
	if (prev && (prev->type == REDI_IN || prev->type == REDI_OT
			|| prev->type == APPEND || prev->type == HEREDOC))
		return (lex_syntax_error(i, "newline"), FALSE);
	return (TRUE);
}
