/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 23:30:41 by amtan             #+#    #+#             */
/*   Updated: 2026/03/18 00:26:22 by amtan            ###   ########.fr       */
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

int	ms_exec_cmd_path(t_info *i, t_ast *cmd, const char *name, char *path)
{
	pid_t	pid;
	int		code;

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
			ms_exit_child(i, 1);
		execve(path, cmd->args, i->my_env);
		code = ms_exec_child_fail(name, path);
		if (path != name)
			ft_sfree((void **)&path);
		ms_exit_child(i, code);
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
	if (ast->type != AST_CMD)
		return ;
	if (!ast->args || !ast->args[0])
		return ((void)(i->err = ms_exec_redir_only(ast)));
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
