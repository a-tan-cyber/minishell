/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hd_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:43:24 by amtan             #+#    #+#             */
/*   Updated: 2026/03/07 17:24:17 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ms_hd_cleanup_list(t_redir *rdir)
{
	while (rdir)
	{
		if (rdir->is_hd_tmp && rdir->file)
			unlink(rdir->file);
		rdir = rdir->next;
	}
}

void	ms_hd_warn_eof(t_redir *r)
{
	ft_putstr_fd("moonshell: warning: here-document at line ",
		STDERR_FILENO);
	ft_putnbr_fd(r->line_no, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(r->file, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

void	ms_heredoc_cleanup_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->left)
		ms_heredoc_cleanup_ast(ast->left);
	if (ast->riht)
		ms_heredoc_cleanup_ast(ast->riht);
	if (ast->type == AST_CMD && ast->rdir)
		ms_hd_cleanup_list(ast->rdir);
}
