/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:26:56 by amtan             #+#    #+#             */
/*   Updated: 2026/03/13 16:51:30 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_hd_read_to_fd(t_info *i, t_redir *r, int fd, char *delim)
{
	char	*line;

	while (TRUE)
	{
		line = ms_input_next_line(i, "> ");
		if (g_sig == SIGINT)
			return (ft_sfree((void **)&i->input_buf), free(line), 1);
		if (!line)
			return (ms_hd_warn_eof(r->line_no, delim), 0);
		i->line_no++;
		if (ms_hd_is_delim(line, delim))
			return (free(line), 0);
		if (!ms_hd_write_line(i, fd, line, r->hd_expand))
			return (free(line), i->err = 1, 1);
		free(line);
	}
}

static int	ms_hd_one(t_info *i, t_redir *r, int *idx)
{
	int		fd;
	char	*path;
	char	*delim;

	path = ms_hd_path((*idx)++);
	if (!path)
		return (i->err = 1, 1);
	unlink(path);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("moonshell: heredoc: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (ft_sfree((void **)&path), i->err = 1, 1);
	}
	delim = r->file;
	r->file = path;
	i->hd_delim = delim;
	if (ms_hd_read_to_fd(i, r, fd, delim))
	{
		i->hd_delim = NULL;
		return (ms_hd_fail_one(i, r, &delim, fd));
	}
	return (ms_hd_finish_ok(i, r, &delim, fd));
}

static int	ms_hd_prepare_list(t_info *i, t_redir *r, int *idx)
{
	while (r)
	{
		if (r->type == HEREDOC && ms_hd_one(i, r, idx))
			return (1);
		r = r->next;
	}
	return (0);
}

static int	ms_hd_prepare_ast_rec(t_info *i, t_ast *ast, int *idx)
{
	if (!ast)
		return (0);
	if (ast->left && ms_hd_prepare_ast_rec(i, ast->left, idx))
		return (1);
	if (ast->riht && ms_hd_prepare_ast_rec(i, ast->riht, idx))
		return (1);
	if (ast->type == AST_CMD && ast->rdir)
		return (ms_hd_prepare_list(i, ast->rdir, idx));
	return (0);
}

int	ms_heredoc_prepare_ast(t_info *i, t_ast *ast)
{
	int	idx;

	if (!i || !ast)
		return (0);
	idx = 0;
	return (ms_hd_prepare_ast_rec(i, ast, &idx));
}
