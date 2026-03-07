/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 23:30:41 by amtan             #+#    #+#             */
/*   Updated: 2026/03/07 10:33:07 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_wait_exit_status(t_info *i, pid_t pid, t_bool print_msg)
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
	return (ms_wait_status(status, i->interactive, print_msg));
}

int	ms_exec_cmd_path(t_info *i, t_ast *cmd, const char *name, const char *path)
{
	pid_t	pid;

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
		if (cmd->rdir && ms_redir_apply(cmd->rdir))
			exit(1);
		execve(path, cmd->args, i->my_env);
		ms_exec_child_fail(name, path);
	}
	return (ms_wait_exit_status(i, pid, TRUE));
}

static void	ms_exec_node(t_info *i, t_ast *ast);

static void	ms_exec_logic(t_info *i, t_ast *ast)
{
	ms_exec_node(i, ast->left);
	if (ast->type == AST_AND && i->err == 0)
		ms_exec_node(i, ast->riht);
	if (ast->type == AST_OR && i->err != 0)
		ms_exec_node(i, ast->riht);
}

static void	ms_exec_node(t_info *i, t_ast *ast)
{
	if (!i || !ast)
		return ;
	if (ast->type == AST_AND || ast->type == AST_OR)
		return (ms_exec_logic(i, ast));
	if (ast->type == AST_BRKT)
	{
		i->err = ms_exec_brkt(i, ast);
		return ;
	}
	if (ast->type == AST_PIPE)
	{
		i->err = ms_exec_pipe(i, ast);
		return ;
	}
	if (ast->type != AST_CMD || !ast->args || !ast->args[0])
		return ;
	ms_exec_cmd(i, ast);
}

void	ms_exec_ast(t_info *i, t_ast *ast)
{
	if (!i || !ast)
		return ;
	if (ms_heredoc_prepare_ast(i, ast))
		return (ms_heredoc_cleanup_ast(ast));
	ms_exec_node(i, ast);
	ms_heredoc_cleanup_ast(ast);
}
