/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:24:08 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 17:23:55 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_exec_cmd_pre(t_info *i, t_ast *ast)
{
	if (ms_try_assign_only(i, ast))
		return (1);
	if (ms_try_assign_exec(i, ast))
		return (1);
	if (ms_try_assign_builtin(i, ast))
		return (1);
	if (ms_try_assign_cd(i, ast))
		return (1);
	if (ms_try_builtin(i, ast))
		return (1);
	return (0);
}

void	ms_exec_cmd(t_info *i, t_ast *ast)
{
	char	*path;

	if (ms_exec_cmd_pre(i, ast))
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
