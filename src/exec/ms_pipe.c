/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:01:34 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:43:23 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_wait_exit_status(t_info *i, pid_t pid, t_bool print_msg)
{
	int	status;

	if (ms_wait_pid(pid, &status, 0))
		return (1);
	return (ms_wait_status(status, i->interactive, print_msg));
}

static void	ms_exec_cmd_child(t_info *i, t_ast *cmd)
{
	char	*path;
	int		code;

	set_child_signals();
	if (ms_try_assign_exec(i, cmd) || ms_try_assign_builtin(i, cmd)
		|| ms_try_assign_export(i, cmd) || ms_try_builtin(i, cmd))
		ms_exit_child(i, i->err);
	if (cmd->rdir && ms_redir_apply(cmd->rdir))
		ms_exit_child(i, 1);
	if (ft_strchr(cmd->args[0], '/'))
		execve(cmd->args[0], cmd->args, i->my_env);
	path = ms_resolve_path(i, cmd->args[0]);
	if (!path)
	{
		ms_cmd_not_found(cmd->args[0]);
		ms_exit_child(i, 127);
	}
	execve(path, cmd->args, i->my_env);
	code = ms_exec_child_fail(cmd->args[0], path);
	ft_sfree((void **)&path);
	ms_exit_child(i, code);
}

static pid_t	ms_pipe_fork_side(t_info *i, t_ast *sub, int fd[2], int is_left)
{
	pid_t	pid;
	int		c0;
	int		c1;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if ((is_left && dup2(fd[1], STDOUT_FILENO) < 0)
			|| (!is_left && dup2(fd[0], STDIN_FILENO) < 0))
			ms_exit_child(i, 1);
		c0 = close(fd[0]);
		c1 = close(fd[1]);
		if (c0 < 0 || c1 < 0)
			ms_exit_child(i, 1);
		if (sub && sub->type == AST_CMD && sub->args && sub->args[0])
			ms_exec_cmd_child(i, sub);
		set_child_signals();
		ms_exec_ast(i, sub);
		ms_exit_child(i, i->err);
	}
	return (pid);
}

static int	ms_pipe_wait_parent(t_info *i, pid_t lpid, pid_t rpid)
{
	int	rstatus;

	if (rpid < 0)
		rstatus = ms_wait_exit_status(i, lpid, FALSE);
	else
	{
		rstatus = ms_wait_exit_status(i, rpid, TRUE);
		ms_wait_exit_status(i, lpid, FALSE);
	}
	if (i->interactive)
		set_signals();
	return (rstatus);
}

int	ms_exec_pipe(t_info *i, t_ast *ast)
{
	int		fd[2];
	pid_t	lpid;
	pid_t	rpid;

	if (!i || !ast || !ast->left || !ast->riht)
		return (1);
	if (i->interactive && set_wait_signals())
		return (1);
	if (pipe(fd) < 0)
		return (ms_restore_signals_ret(i, 1));
	lpid = ms_pipe_fork_side(i, ast->left, fd, 1);
	if (lpid < 0)
		return (close(fd[0]), close(fd[1]),
			ms_restore_signals_ret(i, 1));
	rpid = ms_pipe_fork_side(i, ast->riht, fd, 0);
	if (rpid < 0)
		return (close(fd[0]), close(fd[1]),
			ms_pipe_wait_parent(i, lpid, -1));
	close(fd[0]);
	close(fd[1]);
	return (ms_pipe_wait_parent(i, lpid, rpid));
}
