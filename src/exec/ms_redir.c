/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:32:12 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:52:06 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_redir_err(const char *file)
{
	ft_putstr_fd("moonshell: ", STDERR_FILENO);
	ft_putstr_fd((char *)file, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

static int	ms_redir_dup(t_redir *r)
{
	int	fd;
	int	ret;

	if (!r || !r->file)
		return (1);
	if (r->ambig)
		return (ms_ambiguous_redirect(r->word));
	if (r->type == REDI_IN)
		fd = open(r->file, O_RDONLY);
	else if (r->type == REDI_OT)
		fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->type == APPEND)
		fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (ft_putendl_fd("moonshell: redirection not supported",
				STDERR_FILENO), 1);
	if (fd < 0)
		return (ms_redir_err(r->file));
	if (r->type == REDI_IN)
		ret = dup2(fd, STDIN_FILENO);
	else
		ret = dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0 || ret < 0)
		return (ms_redir_err(r->file));
	return (0);
}

int	ms_redir_apply(t_redir *rdir)
{
	while (rdir)
	{
		if (ms_redir_dup(rdir))
			return (1);
		rdir = rdir->next;
	}
	return (0);
}

int	ms_redir_parent_end(int save_in, int save_out)
{
	int	err;
	int	ret;

	err = 0;
	if (save_in >= 0)
	{
		ret = dup2(save_in, STDIN_FILENO);
		if (close(save_in) < 0 || ret < 0)
			err = 1;
	}
	if (save_out >= 0)
	{
		ret = dup2(save_out, STDOUT_FILENO);
		if (close(save_out) < 0 || ret < 0)
			err = 1;
	}
	return (err);
}

int	ms_redir_parent_begin(t_redir *rdir, int *save_in, int *save_out)
{
	if (!save_in || !save_out)
		return (1);
	*save_in = -1;
	*save_out = -1;
	if (!rdir)
		return (0);
	*save_in = dup(STDIN_FILENO);
	if (*save_in < 0)
		return (1);
	*save_out = dup(STDOUT_FILENO);
	if (*save_out < 0)
		return (close(*save_in), *save_in = -1, 1);
	if (ms_redir_apply(rdir))
	{
		ms_redir_parent_end(*save_in, *save_out);
		*save_in = -1;
		*save_out = -1;
		return (1);
	}
	return (0);
}
