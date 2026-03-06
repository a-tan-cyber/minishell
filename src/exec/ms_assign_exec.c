/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_assign_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:19:10 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 16:56:51 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_assign_is_builtin(const char *s)
{
	if (!ft_strcmp(s, "echo") || !ft_strcmp(s, "pwd"))
		return (1);
	if (!ft_strcmp(s, "cd") || !ft_strcmp(s, "env"))
		return (1);
	if (!ft_strcmp(s, "unset") || !ft_strcmp(s, "export"))
		return (1);
	if (!ft_strcmp(s, "exit"))
		return (1);
	return (0);
}

static int	ms_assign_make_temp(t_info *tmp, t_info *src, char **argv, int n)
{
	*tmp = *src;
	tmp->vars = ms_var_dup(src->vars);
	if (!tmp->vars && src->vars)
		return (1);
	tmp->my_env = NULL;
	if (ms_assign_apply_n(&tmp->vars, argv, n, TRUE))
		return (ms_var_clear(&tmp->vars), 1);
	if (ms_var_sync_env(tmp))
		return (ms_var_clear(&tmp->vars), 1);
	return (0);
}

static void	ms_assign_free_temp(t_info *tmp)
{
	if (!tmp)
		return ;
	free_arr(tmp->my_env);
	tmp->my_env = NULL;
	ms_var_clear(&tmp->vars);
}

static int	ms_assign_temp_status(t_info *tmp, t_ast *cmd)
{
	char	*path;

	if (ft_strchr(cmd->args[0], '/'))
		return (ms_exec_cmd_path(tmp, cmd, cmd->args[0], cmd->args[0]));
	path = ms_resolve_path(tmp, cmd->args[0]);
	if (!path)
		return (ms_cmd_not_found(cmd->args[0]), 127);
	tmp->err = ms_exec_cmd_path(tmp, cmd, cmd->args[0], path);
	ft_sfree((void **)&path);
	return (tmp->err);
}

int	ms_try_assign_exec(t_info *i, t_ast *cmd)
{
	int		n;
	t_info	tmp;
	t_ast	fake;

	if (!i || !cmd || !cmd->args || !cmd->args[0])
		return (0);
	n = ms_assign_prefix_len(cmd->args);
	if (n == 0 || !cmd->args[n] || ms_assign_is_builtin(cmd->args[n]))
		return (0);
	if (ms_assign_make_temp(&tmp, i, cmd->args, n))
		return (i->err = 1, 1);
	fake = *cmd;
	fake.args = cmd->args + n;
	i->err = ms_assign_temp_status(&tmp, &fake);
	ms_assign_free_temp(&tmp);
	return (1);
}
