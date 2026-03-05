/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 23:30:41 by amtan             #+#    #+#             */
/*   Updated: 2026/03/05 23:43:26 by amtan            ###   ########.fr       */
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

static int	ms_exec_cmd_path(t_info *i, t_ast *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(cmd->args[0], cmd->args, i->my_env);
		ms_exec_child_fail(cmd->args[0]);
	}
	return (ms_wait_exit_status(pid));
}

void	ms_exec_ast(t_info *i, t_ast *ast)
{
	if (!i || !ast)
		return ;
	if (ast->type != AST_CMD || !ast->args || !ast->args[0])
		return ;
	if (!ft_strchr(ast->args[0], '/'))
		return ;
	i->err = ms_exec_cmd_path(i, ast);
}
