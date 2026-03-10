/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 23:39:35 by amtan             #+#    #+#             */
/*   Updated: 2026/03/10 15:24:31 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_cmd_not_found(const char *cmd)
{
	ft_putstr_fd("moonshell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

int	ms_ambiguous_redirect(const char *file)
{
	ft_putstr_fd("moonshell: ", STDERR_FILENO);
	ft_putstr_fd((char *)file, STDERR_FILENO);
	ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
	return (1);
}

void	ms_exec_child_fail(const char *name, const char *path)
{
	struct stat	st;

	ft_putstr_fd("moonshell: ", STDERR_FILENO);
	ft_putstr_fd((char *)name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (path && stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		ft_putendl_fd("Is a directory", STDERR_FILENO);
	else
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

int	ms_restore_signals_ret(t_info *i, int ret)
{
	if (i && i->interactive)
		set_signals();
	return (ret);
}

int	ms_exec_redir_only(t_ast *ast)
{
	int	save_in;
	int	save_out;

	if (!ast || !ast->rdir)
		return (0);
	if (ms_redir_parent_begin(ast->rdir, &save_in, &save_out))
		return (1);
	ms_redir_parent_end(save_in, save_out);
	return (0);
}
