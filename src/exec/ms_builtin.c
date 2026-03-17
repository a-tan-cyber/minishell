/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 00:23:44 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:41:08 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_builtin_id(const char *s)
{
	if (!ft_strcmp(s, "echo"))
		return (1);
	if (!ft_strcmp(s, "pwd"))
		return (2);
	if (!ft_strcmp(s, "cd"))
		return (3);
	if (!ft_strcmp(s, "env"))
		return (4);
	if (!ft_strcmp(s, "unset"))
		return (5);
	if (!ft_strcmp(s, "export"))
		return (6);
	if (!ft_strcmp(s, "exit"))
		return (7);
	return (0);
}

static void	ms_run_builtin(t_info *i, t_ast *cmd, int id)
{
	if (id == 1)
		i->err = ms_builtin_echo(cmd->args);
	else if (id == 2)
		i->err = ms_builtin_pwd();
	else if (id == 3)
		i->err = ms_builtin_cd(i, cmd->args);
	else if (id == 4)
		i->err = ms_builtin_env(i);
	else if (id == 5)
		i->err = ms_builtin_unset(i, cmd->args);
	else if (id == 6)
		i->err = ms_builtin_export(i, cmd->args);
	else
		ms_builtin_exit(i, cmd->args);
}

int	ms_try_builtin(t_info *i, t_ast *cmd)
{
	int	id;
	int	save_in;
	int	save_out;

	if (!i || !cmd || !cmd->args || !cmd->args[0])
		return (0);
	id = ms_builtin_id(cmd->args[0]);
	if (!id)
		return (0);
	if (ms_redir_parent_begin(cmd->rdir, &save_in, &save_out))
		return (i->err = 1, 1);
	ms_run_builtin(i, cmd, id);
	if (ms_redir_parent_end(save_in, save_out))
		i->err = 1;
	return (1);
}
