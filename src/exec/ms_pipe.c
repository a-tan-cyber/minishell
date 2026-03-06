/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:01:34 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 13:21:05 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_wait_exit_status(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	ms_exec_cmd_child(t_info *i, t_ast *cmd)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (ms_try_builtin(i, cmd))
		exit(i->err);
	if (cmd->rdir && ms_redir_apply(cmd->rdir))
		exit(1);
	if (ft_strchr(cmd->args[0], '/'))
		execve(cmd->args[0], cmd->args, i->my_env);
	else
	{
		path = ms_resolve_path(i, cmd->args[0]);
		if (!path)
			return (ms_cmd_not_found(cmd->args[0]), exit(127));
		execve(path, cmd->args, i->my_env);
	}
	ms_exec_child_fail(cmd->args[0], cmd->args[0]);
}

static void	ms_exec_ast_child(t_info *i, t_ast *ast)
{
	if (!ast)
		exit(1);
	if (ast->type == AST_CMD && ast->args && ast->args[0])
		ms_exec_cmd_child(i, ast);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ms_exec_ast(i, ast);
	exit(i->err);
}

static pid_t	ms_pipe_fork_side(t_info *i, t_ast *sub, int fd[2], int is_left)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (is_left)
			dup2(fd[1], STDOUT_FILENO);
		else
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		ms_exec_ast_child(i, sub);
		exit(1);
	}
	return (pid);
}

int	ms_exec_pipe(t_info *i, t_ast *ast)
{
	int		fd[2];
	pid_t	lpid;
	pid_t	rpid;
	int		rstatus;

	if (!i || !ast || !ast->left || !ast->riht)
		return (1);
	if (pipe(fd) < 0)
		return (1);
	lpid = ms_pipe_fork_side(i, ast->left, fd, 1);
	if (lpid < 0)
		return (close(fd[0]), close(fd[1]), 1);
	rpid = ms_pipe_fork_side(i, ast->riht, fd, 0);
	if (rpid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		ms_wait_exit_status(lpid);
		return (1);
	}
	close(fd[0]);
	close(fd[1]);
	rstatus = ms_wait_exit_status(rpid);
	ms_wait_exit_status(lpid);
	return (rstatus);
}
