/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_brkt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:19:29 by amtan             #+#    #+#             */
/*   Updated: 2026/03/13 14:54:22 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_brkt_wait_parent(t_info *i, pid_t pid)
{
	int	status;

	if (ms_wait_pid(pid, &status, 0))
	{
		if (i->interactive)
			set_signals();
		return (1);
	}
	if (i->interactive)
		set_signals();
	return (ms_wait_status(status, i->interactive, TRUE));
}

int	ms_exec_brkt(t_info *i, t_ast *ast)
{
	pid_t	pid;

	if (!i || !ast || !ast->left)
		return (1);
	if (i->interactive && set_wait_signals())
		return (1);
	pid = fork();
	if (pid < 0)
	{
		if (i->interactive)
			set_signals();
		return (1);
	}
	if (pid == 0)
	{
		set_child_signals();
		ms_exec_ast(i, ast->left);
		ms_exit_child(i, i->err);
	}
	return (ms_brkt_wait_parent(i, pid));
}
