/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 23:30:41 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 11:46:41 by amtan            ###   ########.fr       */
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

static int	ms_exec_cmd_path(t_info *i, t_ast *cmd, const char *name,
			const char *path)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (cmd->rdir && ms_redir_apply(cmd->rdir))
			exit(1);
		execve(path, cmd->args, i->my_env);
		ms_exec_child_fail(name, path);
	}
	return (ms_wait_exit_status(pid));
}

static void	ms_exec_cmd(t_info *i, t_ast *ast)
{
	char	*path;

	if (ms_try_builtin(i, ast))
		return ;
	if (ft_strchr(ast->args[0], '/'))
	{
		i->err = ms_exec_cmd_path(i, ast, ast->args[0], ast->args[0]);
		return ;
	}
	path = ms_resolve_path(i, ast->args[0]);
	if (!path)
	{
		ms_cmd_not_found(ast->args[0]);
		i->err = 127;
		return ;
	}
	i->err = ms_exec_cmd_path(i, ast, ast->args[0], path);
	ft_sfree((void **)&path);
}

void	ms_exec_ast(t_info *i, t_ast *ast)
{
	if (!i || !ast)
		return ;
	if (ms_heredoc_prepare_ast(i, ast))
		return ;
	if (ast->type == AST_PIPE)
	{
		i->err = ms_exec_pipe(i, ast);
		return ;
	}
	if (ast->type != AST_CMD || !ast->args || !ast->args[0])
		return ;
	ms_exec_cmd(i, ast);
}
