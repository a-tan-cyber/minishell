/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hd_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:43:24 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:43:46 by amtan            ###   ########.fr       */
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

void	ms_hd_warn_eof(int line_no, const char *delim)
{
	ft_putstr_fd("moonshell: warning: here-document at line ",
		STDERR_FILENO);
	ft_putnbr_fd(line_no, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)delim, STDERR_FILENO);
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

int	ms_hd_finish_ok(t_info *i, t_redir *r, char **delim, int fd)
{
	if (close(fd) < 0)
		return (ms_hd_fail_one(i, r, delim, -1));
	ft_sfree((void **)delim);
	i->hd_delim = NULL;
	r->type = REDI_IN;
	r->is_hd_tmp = TRUE;
	return (0);
}
